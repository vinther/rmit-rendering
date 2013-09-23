/*
 * FrameBuffer.hpp
 *
 *  Created on: 19/08/2013
 *      Author: svp
 */

#ifndef FRAMEBUFFER_HPP_
#define FRAMEBUFFER_HPP_

#include <memory>
#include <vector>

typedef unsigned int GLuint;
typedef unsigned int GLenum;
typedef int GLint;

namespace renderer {
namespace resources {

class Texture;

class frame_buffer {
public:
    typedef std::pair<GLenum, std::unique_ptr<Texture>> attachment_type;
    typedef std::pair<GLenum, GLenum> binding_type;

	frame_buffer();
	virtual ~frame_buffer();

	GLuint buffer;

	void attach(std::unique_ptr<Texture> texture, GLenum attachmentPoint);

	void enableAll();
	void enable(const std::initializer_list<GLenum> attachments);
	void disable();
	void bindTextures(const std::initializer_list<binding_type>& bindings);

	std::vector<attachment_type> attachments;
};

} /* namespace resources */
} /* namespace renderer */

#endif /* FRAMEBUFFER_HPP_ */
