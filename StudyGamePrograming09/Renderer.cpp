#include "Renderer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include <glew.h>
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "Shader.h"
#include "VertexInfo.h"
#include "Texture.h"
#include "Mesh.h"

Renderer::Renderer(Game* game)
	: mGame(game)
	, mWindow(nullptr)
	, mSpriteShader(nullptr)
	//, mMeshShader(nullptr)
	, mVertexInfo(nullptr)
{}

Renderer::~Renderer()
{}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	// OpenGL�����ݒ�
	// OpenGL�̃R�A�v���t�@�C�����g�p
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// version 3.3���w��
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// �e�J���[�`�����l��8�r�b�g(RGBA)
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// �_�u���o�b�t�@�L����
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// OpenGL�̃n�[�h�E�F�A�A�N�Z�����[�^�g�p
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// SDL�E�B���h�E���쐬
	mWindow = SDL_CreateWindow(
		"Game Programming in C++",
		100,
		100,
		static_cast<int>(screenWidth),
		static_cast<int>(screenHeight),
		SDL_WINDOW_OPENGL					//SDL�E�B���h�E��OpenGL���g�p����ۂɎw��
	);

	if (!mWindow)
	{
		SDL_Log("�E�B���h�E�̍쐬�Ɏ��s���܂���: %s", SDL_GetError());
		return false;
	}
	
	// OpenGL�R���e�N�X�g�𐶐��i���ׂĂ�OpenGL�@�\�ɃA�N�Z�X����j
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEW��������
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("GLEW�̏������Ɏ��s���܂����B");
		return false;
	}
	glGetError();

	// �o�[�e�b�N�X�z��I�u�W�F�N�g�̐���
	CreateVertexInfo();

	// �V�F�[�_�[�̐���
	if (!LoadShaders())
	{
		SDL_Log("�V�F�[�_�[�̓ǂݍ��݂Ɏ��s���܂����B");
		return false;
	}

	// ��ʃN���A�̐F��ݒ�
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	return true;
}

void Renderer::UnloadData()
{
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Shutdown()
{
	delete mVertexInfo;
	mSpriteShader->Unload();
	delete mSpriteShader;
	//mMeshShader->Unload();
	//delete mMeshShader;
	for (auto sh : mShaders)
	{
		sh.first->Unload();
		delete sh.first;
	}
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}


void Renderer::Draw()
{
	// ��ʂ��N���A & �[�x�o�b�t�@���N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// �[�x�L�����A�A���t�@�u�����f�B���O������
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	// �V�F�[�_�[���ɕ`��
	for (auto sh : mShaders)
	{
		// ���b�V����`��
		//mMeshShader->SetActive();
		//mMeshShader->SetMatrixUniform("uViewProj", mView * mProj);
		sh.first->SetActive();
		sh.first->SetMatrixUniform("uViewProj", mView * mProj);
		// ������Uniform�ϐ����X�V
		//SetLightUniforms(mMeshShader);
		SetLightUniforms(sh.first);

		for (auto mc : mMeshComps)
		{
			std::string shaderName = mc->GetMesh()->GetShaderName();
			if (shaderName == sh.second)
			{
				mc->Draw(sh.first);
			}
		}
	}

	// �[�x�������A�A���t�@�u�����f�B���O�L����
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	// �X�v���C�g��`��
	mSpriteShader->SetActive();
	mVertexInfo->SetActive();
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	// �_�u���o�b�t�@������
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
void Renderer::AddMeshComp(MeshComponent* mesh)
{
	mMeshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
	mMeshComps.erase(iter);
}

class Texture* Renderer::GetTexture(const std::string& filename)
{
	class Texture* tex = nullptr;
	auto iter = mTextures.find(filename);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(filename))
		{
			mTextures.emplace(filename, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}
Mesh* Renderer::GetMesh(const std::string& fileName)
{
	class Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			mMeshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

void Renderer::CreateVertexInfo()
{
	// ������W�n
	// ���_���W(vector3), �@���x�N�g��(Vector3), �e�N�X�`�����W(Vector2)
	float vertices[] = {
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,	 // top left
		 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,	 // top right
		 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,	 // bottom right
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f		 // bottom left
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	mVertexInfo = new VertexInfo(vertices, 4, indices, 6);
}

bool Renderer::LoadShaders()
{
	// �X�v���C�g�p�V�F�[�_�[�𐶐�
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/SpriteShader.vert", "Shaders/SpriteShader.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();
	// �X�v���C�g�̕`��ɂ́A���s�ˉe���s���B
	mView = Matrix4::Identity;
	mProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", mView * mProj);
	
	// ���b�V���p�V�F�[�_�[�𐶐�
	// ���b�V���̕`��ɂ́A�����ˉe���s���B
	Vector3 cameraPos = Vector3::Zero;
	Vector3 cameraTarget = Vector3::UnitX;
	Vector3 cameraUp = Vector3::UnitZ;
	mView = Matrix4::CreateLookAt(cameraPos, cameraTarget, cameraUp);
	mProj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), mScreenWidth, mScreenHeight, 0.01f, 10000.0f);
	//BasicShader
	class Shader* mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/BasicMesh.vert", "Shaders/BasicMesh.frag"))
	{
		return false;
	}
	mMeshShader->SetActive();
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProj);
	mShaders.emplace(mMeshShader, "BasicMesh");
	//LambertShader
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/LambertMesh.vert", "Shaders/LambertMesh.frag"))
	{
		return false;
	}
	mMeshShader->SetActive();
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProj);
	mShaders.emplace(mMeshShader, "LambertMesh");
	//PhongShader
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/PhongMesh.vert", "Shaders/PhongMesh.frag"))
	{
		return false;
	}
	mMeshShader->SetActive();
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProj);
	mShaders.emplace(mMeshShader, "PhongMesh");

	/*
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/MeshShader.vert", "Shaders/MeshShader.frag"))
	{
		return false;
	}
	mMeshShader->SetActive();
	*/
	
	return true;
}

void Renderer::SetLightUniforms(Shader* shader)
{
	// �J�����̈ʒu�̓r���[�𔽓]���ċ��߂�
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	// ����
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	// ���s����
	/*
	shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
	*/
	std::string str;
	for (int i = 0; i < mDirLights.size(); i++)
	{
		str = "uDirLights[" + std::to_string(i) + "].mDirection";
		shader->SetVectorUniform(str.c_str(), mDirLights[i].mDirection);
		str = "uDirLights[" + std::to_string(i) + "].mDiffuseColor";
		shader->SetVectorUniform(str.c_str(), mDirLights[i].mDiffuseColor);
		str = "uDirLights[" + std::to_string(i) + "].mSpecColor";
		shader->SetVectorUniform(str.c_str(), mDirLights[i].mSpecColor);
	}
	// �_����
	for (int i = 0; i < mPointLights.size(); i++)
	{
		str = "uPointLights[" + std::to_string(i) + "].mPosition";
		shader->SetVectorUniform(str.c_str(), mPointLights[i].mPosition);
		str = "uPointLights[" + std::to_string(i) + "].mDiffuseColor";
		shader->SetVectorUniform(str.c_str(), mPointLights[i].mDiffuseColor);
		str = "uPointLights[" + std::to_string(i) + "].mSpecColor";
		shader->SetVectorUniform(str.c_str(), mPointLights[i].mSpecColor);
		str = "uPointLights[" + std::to_string(i) + "].mAttenuation";
		shader->SetFloatUniform(str.c_str(), mPointLights[i].mAttenuation);
	}

	// �X�|�b�g���C�g
	for (int i = 0; i < mSpotLights.size(); i++)
	{
		str = "uSpotLights[" + std::to_string(i) + "].mPosition";
		shader->SetVectorUniform(str.c_str(), mSpotLights[i].mPosition);
		str = "uSpotLights[" + std::to_string(i) + "].mDirection";
		shader->SetVectorUniform(str.c_str(), mSpotLights[i].mDirection);
		str = "uSpotLights[" + std::to_string(i) + "].mDiffuseColor";
		shader->SetVectorUniform(str.c_str(), mSpotLights[i].mDiffuseColor);
		str = "uSpotLights[" + std::to_string(i) + "].mSpecColor";
		shader->SetVectorUniform(str.c_str(), mSpotLights[i].mSpecColor);
		str = "uSpotLights[" + std::to_string(i) + "].mAttenuation";
		shader->SetFloatUniform(str.c_str(), mSpotLights[i].mAttenuation);
		str = "uSpotLights[" + std::to_string(i) + "].mCornAngle";
		shader->SetFloatUniform(str.c_str(), mSpotLights[i].mCornAngle);
		str = "uSpotLights[" + std::to_string(i) + "].mFalloff";
		shader->SetFloatUniform(str.c_str(), mSpotLights[i].mFalloff);
	}
}