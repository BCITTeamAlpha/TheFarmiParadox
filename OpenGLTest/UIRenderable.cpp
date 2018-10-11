#include "UIRenderable.h"

GLuint UIRenderable::count = 0;

UIRenderable::UIRenderable() : _id(UIRenderable::count++) {}

glm::vec2 UIRenderable::charToVec2(char c) {
	int c_value = int(c);

	int index = 47; // default to ' '
	if (c_value>64 && c_value<91) // A-Z
		index = c_value - 65;
	else if (c_value>96 && c_value<123) // a-z
		index = c_value - 97;
	else if (c_value>47 && c_value<58) // 0-9
		index = c_value - 22;
	else if (c_value == 33) // !
		index = 36;
	else if (c_value == 63) // ?
		index = 37;
	else if (c_value == 43) // +
		index = 38;
	else if (c_value == 45) // -
		index = 39;
	else if (c_value == 61) // =
		index = 40;
	else if (c_value == 58) // :
		index = 41;
	else if (c_value == 46) // .
		index = 42;
	else if (c_value == 44) // ,
		index = 43;
	else if (c_value == 42) // *
		index = 44;
	else if (c_value == 36) // $
		index = 45;

	glm::vec2 ret;
	ret.x = 0.0f + 0.125f * (index % 8);
	ret.y = 1.0f - 0.125f * (index / 8);
	return ret;
}

void UIRenderable::BuildWithString(std::string text) {
	std::string string = "Text rendering kind of working?";
	for (int i = 0; i < string.size(); i++) {
		glm::vec2 UV_topLeft = charToVec2(string[i]);
		UV_topLeft;
		_positions.push_back(glm::vec2(32 * i, 32)); // top-left
		_positions.push_back(glm::vec2(32 * i + 32, 32)); //top-right
		_positions.push_back(glm::vec2(32 * i, 0)); // bottom-left
		_positions.push_back(glm::vec2(32 * i + 32, 0)); // bottom-right
		_texCoords.push_back(UV_topLeft);
		_texCoords.push_back(UV_topLeft + glm::vec2(0.125, 0));
		_texCoords.push_back(UV_topLeft + glm::vec2(0, -0.125));
		_texCoords.push_back(UV_topLeft + glm::vec2(0.125, -0.125));
		_elements.push_back(i * 4 + 1);
		_elements.push_back(i * 4 + 0);
		_elements.push_back(i * 4 + 2);
		_elements.push_back(i * 4 + 1);
		_elements.push_back(i * 4 + 2);
		_elements.push_back(i * 4 + 3);
	}
}