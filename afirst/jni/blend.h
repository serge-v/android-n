GLenum src_blend_mode = 1;
GLenum dst_blend_mode = 1;

GLenum src_blend_modes[] = {
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_SRC_ALPHA_SATURATE
};

GLenum dst_blend_modes[] = {
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA
};

void inc_blend_mode(int source)
{
	if (source)
		src_blend_mode = (src_blend_mode + 1) % (sizeof(src_blend_modes) / sizeof(src_blend_modes[0]));
	else
		dst_blend_mode = (dst_blend_mode + 1) % (sizeof(dst_blend_modes) / sizeof(dst_blend_modes[0]));

	glBlendFunc(src_blend_modes[src_blend_mode], dst_blend_modes[dst_blend_mode]);
	LOGI("src blend mode: %d", src_blend_modes[src_blend_mode]);
	LOGI("drc blend mode: %d", dst_blend_modes[dst_blend_mode]);
}
