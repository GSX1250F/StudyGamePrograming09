// OpenGL3.3�ɑΉ�����GLSL�o�[�W�����̎w��

#version 330

// ���͕ϐ��i�R�����j�̈ʒu���̐錾
// ���͈ʒu���W����
in vec3 inPosition;

void main()
{
	// inPosition��gl_Position�ɁA���̂܂ܓn��
	gl_Position = vec4(inPosition, 1.0);
}
