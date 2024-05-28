// Request GLSL 3.3
#version 330

// ���[���h�ϊ��ƃr���[�ˉe�̂��߂�uniform�s��
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// ����0�͈ʒu�A����1�͖@���x�N�g���A2�̓e�N�X�`�����W
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// �o�͂Ƀe�N�X�`�����W��ǉ�
out vec2 fragTexCoord;

void main()
{
	// �ʒu���W�𓯎����W�n�ɕϊ�
	vec4 pos = vec4(inPosition, 1.0);
	// �ʒu�����[���h��ԂɁA�����ăN���b�v��Ԃɕϊ�
	gl_Position = pos * uWorldTransform * uViewProj;

	// �e�N�X�`�����W���t���O�����g�V�F�[�_�[�ɓn��
	fragTexCoord = inTexCoord;
}
