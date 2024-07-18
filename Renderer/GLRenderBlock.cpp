
#include <Common.h>
#include <Renderer/GLRenderBlock.h>
#include <Core/Services/Services.h>
#include <Renderer/Bone.h>
#include <unordered_map>

#ifdef RENDERER_OPENGL
namespace CraftyBlocks
{
	bool RenderBlock::Init()
	{
		m_renderer = Services::GetRenderer();
		glGenBuffers(1, &m_bufferId);
		glGenVertexArrays(1, &m_vertexArrayObject);
		m_bDoneInit = true;
		return true;
	}

	void RenderBlock::Update()
	{
		if (m_animationBufferDirty)
		{
			DumpAnimationData();
			m_animationBufferDirty = false;
		}

		if (m_bufferDirty)
		{
			DumpBufferData();
			m_bufferDirty = false;
		}
	}

	void RenderBlock::Render()
	{
		if (m_mesh == nullptr || m_material == nullptr)
		{
			Log(Logger::ERROR, "Render block missing critical component, mesh or material.");
			return;
		}

		if (m_bufferDirty || m_mesh->m_vertices == nullptr)
		{
			return;
		}

		if (m_material)
		{
			m_material->bind();

			if (m_mesh->m_animationsRef != nullptr)
			{
				if (m_mesh->m_animationsRef->size() > 0)
				{
					float time = Services::GetTime();
					m_material->getShader().setUniformfv("animationTimeElapsed", time);

					int count = 0;

					// Populate animations
					for (std::shared_ptr<Animation> anim : *m_mesh->m_animationsRef)
					{
						if (m_animationBuffer)
						{
							m_material->getShader().bindShaderStorageBuffer("Animations", m_animationBuffer);
							m_material->getShader().setUniform1i("animationCount", anim->getBoneAnimations().size());
						}

						for (std::shared_ptr<BoneAnim> boneAnim : anim->getBoneAnimations())
						{
							for (int i = 0; i < 2; ++i)
							{
								std::string posKeyKey = "positionKeys[" + std::to_string(count * 2 + i) + "].Key";
								std::string posKeyTime = "positionKeys[" + std::to_string(count * 2 + i) + "].Time";

								std::string scaleKeyKey = "scaleKeys[" + std::to_string(count * 2 + i) + "].Key";
								std::string scaleKeyTime = "scaleKeys[" + std::to_string(count * 2 + i) + "].Time";

								std::string rotationKeyKey = "rotationKeys[" + std::to_string(count * 2 + i) + "].Key";
								std::string rotationKeyTime = "rotationKeys[" + std::to_string(count * 2 + i) + "].Time";

								int index = (int)time % anim->getBoneAnimations().size();

								// Wrap around.
								if (index + i >= boneAnim->GetPositionKeys().size())
								{
									index = 0;
								}

								m_material->getShader().setUniformVec4(posKeyKey, boneAnim->GetPositionKeys()[index + i].Value);
								m_material->getShader().setUniformfv(posKeyTime, boneAnim->GetPositionKeys()[index + i].Time);

								m_material->getShader().setUniformVec4(rotationKeyKey, boneAnim->GetRotationKeys()[index + i].Value);
								m_material->getShader().setUniformfv(rotationKeyTime, boneAnim->GetRotationKeys()[index + i].Time);

								m_material->getShader().setUniformVec4(scaleKeyKey, boneAnim->GetScaleKeys()[index + i].Value);
								m_material->getShader().setUniformfv(scaleKeyTime, boneAnim->GetScaleKeys()[index + i].Time);
							}

							count++;
						}

						break;
					}
				}
			}

			if (m_renderer->isShadowPass())
			{
				Light* activeLightSource = m_renderer->getActiveLight();
				float near_plane = 0.1f, far_plane = 1000.0f;
				glm::mat4 lightProjection;

				// Spot light projection
				if (activeLightSource->getLightType() == LightType::Spot)
				{
					lightProjection = glm::perspectiveLH(activeLightSource->getApature() * 2, 1.0f, near_plane, far_plane);
				}
				else
				{
					lightProjection = glm::perspectiveLH(180.0f, 1.0f, near_plane, far_plane);
				}

				// Get active light and work out view matrix 
				glm::vec3 position = activeLightSource->getTransform()->getWorldPosition();
				glm::vec3 up = activeLightSource->getTransform()->getUp();
				glm::vec3 forward = activeLightSource->getTransform()->getForward();

				glm::mat4 view = glm::lookAtLH(position, position + forward, up);

				m_material->getShader().setUniformBool(m_shadowUniform, true);

				//m_material->getShader().uniformMat4fv("wvpMat", 1, false,
				//	m_renderer->getActiveCamera()->getProjMatrix() * m_renderer->getActiveCamera()->getViewMatrix() * m_transform->getWorldMatrix());

				//m_material->getShader().uniformMat4fv(m_shadowMat, 1, false,
				//	lightProjection * view * m_transform->getWorldMatrix());
				m_material->getShader().uniformMat4fv(m_viewUniform, 1, false, view);
				m_material->getShader().uniformMat4fv(m_modelUniform, 1, false, m_transform->getWorldMatrix());
				m_material->getShader().uniformMat4fv(m_projUniform, 1, false, lightProjection);
			}
			else
			{
				if (m_transform != nullptr)
				{
					// Remove this cheaper todo in shader.
					// Deprecated!!! - to be removed in the future.
					//m_material->getShader().uniformMat4fv("wvpMat", 1, false,
					//	m_renderer->getActiveCamera()->getProjMatrix() * m_renderer->getActiveCamera()->getViewMatrix() * m_transform->getWorldMatrix());

					m_material->getShader().uniformMat4fv(m_modelUniform, 1, false, m_transform->getWorldMatrix());
					m_material->getShader().uniformMat4fv(m_viewUniform, 1, false, m_renderer->getActiveCamera()->getViewMatrix());
					m_material->getShader().uniformMat4fv(m_projUniform, 1, false, m_renderer->getActiveCamera()->getProjMatrix());
				}

				m_material->getShader().setUniformBool(m_shadowUniform, false);
				m_material->getShader().setUniformVec3(m_cameraUniform, m_renderer->getActiveCamera()->getTransform()->getWorldPosition());
				m_material->getShader().setUniformVec3(m_cameraLook, m_renderer->getActiveCamera()->getTransform()->getForward());
			}
		}
		else
		{
			Log(Logger::ERROR, "Render block no material present!");
		}

		// should not do blending here/ or use depth test here
		// See renderqueue in scene (sets depth test)

		if (m_mesh->shouldRenderBackFaces())
		{
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glEnable(GL_CULL_FACE);
		}

		size_t textureBufferSize = m_mesh->getTextureBufferSize();
		size_t texture3dBufferSize = m_mesh->getTexture3dBufferSize();
		if (textureBufferSize == 0 && texture3dBufferSize == 0)
		{
			//m_material->getShader().setUniformBool("hasDiffuseMap", false);
			//m_material->getShader().setUniformBool("generatePlanarUvs", true);
		}

		glLineWidth(m_mesh->GetLineThickness());
		glBindVertexArray(m_vertexArrayObject);
		glDrawArrays(m_primitiveMode, 0, (GLsizei)m_mesh->m_vertices->size());

		LogGraphicsErrors();
	}

	void RenderBlock::SetDrawMode(PrimitiveMode mode)
	{
		// Replace with lookup!
		switch (mode) {

		case PrimitiveMode::POINTS:
			m_primitiveMode = GL_POINTS;
			break;
		case PrimitiveMode::LINES:
			m_primitiveMode = GL_LINES;
			break;
		case PrimitiveMode::LINE_STRIP:
			m_primitiveMode = GL_LINE_STRIP;
			break;
		case PrimitiveMode::LINE_LOOP:
			m_primitiveMode = GL_LINE_LOOP;
			break;
		case PrimitiveMode::TRIANGLES:
			m_primitiveMode = GL_TRIANGLES;
			break;
		case PrimitiveMode::TRIANGLE_STRIP:
			m_primitiveMode = GL_TRIANGLE_STRIP;
			break;
		case PrimitiveMode::TRIANGLE_FAN:
			m_primitiveMode = GL_TRIANGLE_FAN;
			break;

		default:
			Log(Logger::ERROR, "Primitive mode not found defaulting to [GL_TRIANGLES].");
			m_primitiveMode = GL_TRIANGLES;
		}
	}

	void RenderBlock::ConfigureVertexAttributes()
	{
		if (!m_bDoneInit)
		{
			Log(Logger::ERROR, "Attempting to set vertex attributes without initialising render block first.");
		}

		if (m_material == nullptr)
		{
			Log(Logger::ERROR, "Unable to set vertex attributes, a valid material is required!");
			return;
		}

		glBindVertexArray(m_vertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);

		const Shader shader = m_material->getShader();

		/// buffer offsets
		size_t vertexBufferSize = m_mesh->getVertexBufferSize();
		size_t textureBufferSize = m_mesh->getTextureBufferSize();
		size_t texture3dBufferSize = m_mesh->getTexture3dBufferSize();
		size_t colourBufferSize = m_mesh->getColourBufferSize();
		size_t tangentBufferSize = m_mesh->getTangentBufferSize();
		size_t biTangentBufferSize = m_mesh->getBiTangentBufferSize();
		size_t normalBufferSize = m_mesh->getNormalBufferSize();
		size_t weightSize = m_mesh->getWeightBufferSize();

		size_t tBufferSize = (texture3dBufferSize) ? texture3dBufferSize : textureBufferSize;

		// vertex positions
		VertexAttribPointer(shader.getProgram(), shader.VertexPositionAtrib, Vec3Size(), GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		if (texture3dBufferSize)
		{
			VertexAttribPointer(shader.getProgram(), shader.UVSAtrib, Vec3Size(), GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * vertexBufferSize));
		}
		else
		{
			VertexAttribPointer(shader.getProgram(), shader.UVSAtrib, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * vertexBufferSize));
		}

		VertexAttribPointer(shader.getProgram(), shader.ColourAtrib, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * (vertexBufferSize + tBufferSize)));
		VertexAttribPointer(shader.getProgram(), shader.NormalAtrib, Vec3Size(), GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * (vertexBufferSize + tBufferSize + colourBufferSize)));
		VertexAttribPointer(shader.getProgram(), shader.TangentAtrib, Vec3Size(), GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * (vertexBufferSize + tBufferSize + colourBufferSize + normalBufferSize)));
		VertexAttribPointer(shader.getProgram(), shader.BiTangentAtrib, Vec3Size(), GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * (vertexBufferSize + tBufferSize + colourBufferSize + normalBufferSize + tangentBufferSize)));
		VertexAttribPointer(shader.getProgram(), shader.WeightsAtrib, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * (vertexBufferSize + tBufferSize + colourBufferSize + normalBufferSize + tangentBufferSize + biTangentBufferSize)));
		VertexAttribPointer(shader.getProgram(), shader.BoneIdAtrib, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(float) * (vertexBufferSize + tBufferSize + colourBufferSize + normalBufferSize + tangentBufferSize + biTangentBufferSize + weightSize)));

		EnableVertexAttribArray(shader.getProgram(), shader.VertexPositionAtrib);
		EnableVertexAttribArray(shader.getProgram(), shader.UVSAtrib);
		EnableVertexAttribArray(shader.getProgram(), shader.NormalAtrib);
		EnableVertexAttribArray(shader.getProgram(), shader.TangentAtrib);
		EnableVertexAttribArray(shader.getProgram(), shader.BiTangentAtrib);
		EnableVertexAttribArray(shader.getProgram(), shader.WeightsAtrib);
		EnableVertexAttribArray(shader.getProgram(), shader.BoneIdAtrib);

		glBindVertexArray(0);

		LogGraphicsErrors();
	}

	void RenderBlock::SetMaterial(std::shared_ptr<Material> Material)
	{
		m_material = Material;
		m_shadowMat = m_material->GetShader().getUniformLocation("wvpShadowMat");
		m_modelUniform = m_material->GetShader().getUniformLocation("modelMat");
		m_viewUniform = m_material->GetShader().getUniformLocation("viewMat");
		m_projUniform = m_material->GetShader().getUniformLocation("projMat");
		m_shadowUniform = m_material->GetShader().getUniformLocation("shadowPass");
		m_cameraUniform = m_material->GetShader().getUniformLocation("cameraPosition");
		m_cameraLook = m_material->GetShader().getUniformLocation("cameraLook");
	}

	void RenderBlock::VertexAttribPointer(GLuint shaderProgram, std::string name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
	{
		GLint location = glGetAttribLocation(shaderProgram, name.c_str());

		if (location >= 0)
		{
			glVertexAttribPointer(location, size, type, normalized, stride, pointer);
		}

		LogGraphicsErrors();
	}

	GLint RenderBlock::EnableVertexAttribArray(GLuint shaderProgram, std::string name)
	{
		GLint location = glGetAttribLocation(shaderProgram, name.c_str());

		if (location >= 0)
		{
			glEnableVertexAttribArray(location);
		}

		LogGraphicsErrors();
		return location;
	}

	std::vector<uint64> dumpedAnimData;

	GLuint RenderBlock::m_animationBuffer = 0;
	int RenderBlock::m_animationSize = 0;

	void RenderBlock::DumpAnimationData()
	{
		// temporary hack
		if (std::find(dumpedAnimData.begin(), dumpedAnimData.end(), m_material->getShader().UUID.getID()) != dumpedAnimData.end())
		{
			return;
		}

		dumpedAnimData.push_back(m_material->getShader().UUID.getID());

		// hack to be refactored 
		if (m_animationBuffer && m_mesh->m_animationsRef != nullptr)
		{
			m_material->getShader().bindProgram();
			m_material->getShader().bindShaderStorageBuffer("Animations", m_animationBuffer);
			m_material->getShader().setUniform1i("animationCount", m_animationSize);
			return;
		}

		if (m_mesh == nullptr)
		{
			Log(Logger::WARN, "Mesh not assigned!");
			return;
		}

		std::vector<AnimationData> animations;

		if (m_mesh->m_animationsRef != nullptr)
		{
			if (m_mesh->m_animationsRef->size() > 0)
			{
				int count = 0;

				// Populate animations
				for (std::shared_ptr<UAnimation> anim : *m_mesh->m_animationsRef)
				{
					animations.resize(anim->getBoneAnimations().size());

					if (count > 0)
					{
						continue;
					}

					// Needs updating to support multiple animations
					for (std::shared_ptr<BoneAnim> boneAnim : anim->getBoneAnimations())
					{
						animations[count].Duration = anim->getDuration();
						animations[count].TicksPerSecond = anim->getTicksPerSecond();
						animations[count].BoneId = boneAnim->getId();

						for (auto& bone : *m_mesh->m_bones)
						{
							if (bone->getBoneId() == boneAnim->getId())
							{
								//Print("alignment %i", alignof(glm::vec3));
								animations[count].OffsetMatrix = bone->getOffsetMatrix();
								animations[count].ParentBoneId = bone->getParentBoneId();
								break;
							}
						}

						int pCount = 0;

						for (VectorKey key : boneAnim->GetPositionKeys())
						{
							if (pCount >= 256)
							{
								continue;
							}

							//animations[count].PositionKeys[pCount] = key;
							pCount++;
						}

						animations[count].PositionKeyCount = pCount;
						pCount = 0;

						for (VectorKey key : boneAnim->GetScaleKeys())
						{
							if (pCount >= 256)
							{
								continue;
							}

							//animations[count].ScaleKeys[pCount] = key;
							pCount++;
						}

						animations[count].ScaleKeyCount = pCount;
						pCount = 0;

						for (QuatKey key : boneAnim->GetRotationKeys())
						{
							if (pCount >= 256)
							{
								continue;
							}

							//animations[count].RotationKeys[pCount] = key;
							pCount++;
						}

						animations[count].RotationKeyCount = pCount;
						count++;
					}
				}
			}
		}

		if (!animations.size())
		{
			return;
		}

		if (m_animationBuffer == 0)
		{
			glGenBuffers(1, &m_animationBuffer);
		}

		m_animationSize = animations.size();

		m_material->getShader().bindProgram();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_animationBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(AnimationData) * animations.size(), &(animations[0]), GL_STATIC_DRAW);

		m_material->getShader().bindShaderStorageBuffer("Animations", m_animationBuffer);
		m_material->getShader().setUniform1i("animationCount", animations.size());

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		LogGraphicsErrors();
	}

	void RenderBlock::DumpBufferData()
	{
		if (m_mesh == nullptr)
		{
			Log(Logger::WARN, "Mesh not assigned!");
			return;
		}

		SetDrawMode(m_mesh->GetDrawMode());

		size_t vertexBufferSize = m_mesh->GetVertexBufferSize();
		size_t textureBufferSize = m_mesh->GetTextureBufferSize();
		size_t texture3DBufferSize = m_mesh->GetTexture3dBufferSize();
		size_t colourBufferSize = m_mesh->GetColourBufferSize();
		size_t normalBufferSize = m_mesh->GetNormalBufferSize();
		size_t tangentBufferSize = m_mesh->GetTangentBufferSize();
		size_t biTangentBufferSize = m_mesh->GetBiTangentBufferSize();
		size_t weigthSize = m_mesh->GetWeightBufferSize();
		size_t boneIdSize = m_mesh->GetBoneIdBufferSize();

		glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);

		size_t bufferSize =
			vertexBufferSize +
			textureBufferSize +
			texture3DBufferSize +
			colourBufferSize +
			normalBufferSize +
			tangentBufferSize +
			biTangentBufferSize +
			weigthSize +
			boneIdSize;

		/// Dynamic allocate storage based on the buffers the current mesh is using.
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ((bufferSize)), 0, GL_DYNAMIC_DRAW);

		/// Dynamically set the buffer data for the buffers that are currently in use.
		size_t bufferOffset = 0;

		if (vertexBufferSize)
		{
			float* vertices = static_cast<float*>(glm::value_ptr(m_mesh->m_vertices->front()));
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertexBufferSize, &vertices[0]);
			bufferOffset += vertexBufferSize;
		}

		if (textureBufferSize) {
			float* textureCoords = static_cast<float*>(glm::value_ptr(m_mesh->m_uvs->front()));
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * bufferOffset,
				sizeof(float) * textureBufferSize, &textureCoords[0]);
			bufferOffset += textureBufferSize;
		}
		else if (texture3DBufferSize)
		{
			float* textureCoords = static_cast<float*>(glm::value_ptr(m_mesh->m_uvs3d->front()));
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * bufferOffset,
				sizeof(float) * texture3DBufferSize, &textureCoords[0]);
			bufferOffset += textureBufferSize;
		}

		if (colourBufferSize) {
			float* vertexColours = static_cast<float*>(glm::value_ptr(m_mesh->m_colours->front()));
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * bufferOffset,
				sizeof(float) * colourBufferSize, &vertexColours[0]);
			bufferOffset += colourBufferSize;
		}

		if (normalBufferSize) {
			float* normals = static_cast<float*>(glm::value_ptr(m_mesh->m_normals->front()));
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * bufferOffset,
				sizeof(float) * normalBufferSize, &normals[0]);
			bufferOffset += normalBufferSize;
		}

		if (tangentBufferSize) {
			float* tangents = static_cast<float*>(glm::value_ptr(m_mesh->GetTangents()->front()));
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * bufferOffset,
				sizeof(float) * tangentBufferSize, &tangents[0]);
			bufferOffset += tangentBufferSize;
		}

		if (biTangentBufferSize) {
			float* tangents = static_cast<float*>(glm::value_ptr(m_mesh->GetBiTangents()->front()));
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * bufferOffset,
				sizeof(float) * biTangentBufferSize, &tangents[0]);
			bufferOffset += biTangentBufferSize;
		}

		if (weigthSize) {
			float* weights = static_cast<float*>(glm::value_ptr(m_mesh->GetBoneWeights()->front()));
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * bufferOffset,
				sizeof(float) * weigthSize, &weights[0]);
			bufferOffset += weigthSize;
		}

		if (boneIdSize) {
			float* boneids = static_cast<float*>(glm::value_ptr(m_mesh->GetBoneIds()->front()));
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * bufferOffset,
				sizeof(float) * boneIdSize, &boneids[0]);
			bufferOffset += boneIdSize;
		}

		LogGraphicsErrors();
		ConfigureVertexAttributes();
	}
}
#endif