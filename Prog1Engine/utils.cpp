#include "pch.h"
//#define _USE_MATH_DEFINES
//#include <cmath>
#include <algorithm>
#include <iostream>
#include "utils.h"

namespace utils
{
#pragma region OpenGLDrawFunctionality
	void ClearBackground(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void ClearBackground()
	{
		ClearBackground(185.0f / 255.0f, 211.0f / 255.0f, 238.0f / 255.0f);
	}
	void SetColor(float r, float g, float b, float a)
	{
		glColor4f(r, g, b, a);
	}

	void SetColor(const Color4f& color)
	{
		glColor4f(color.r, color.g, color.b, color.a);
	}

	void DrawLine(float x1, float y1, float x2, float y2, float lineWidth)
	{
		glLineWidth(lineWidth);
		glBegin(GL_LINES);
		{
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
		}
		glEnd();
	}

	void DrawLine(const Point2f& p1, const Point2f& p2, float lineWidth)
	{
		DrawLine(p1.x, p1.y, p2.x, p2.y, lineWidth);
	}

	void DrawRect(float left, float top, float width, float height, float lineWidth)
	{
		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			glVertex2f(left, top);
			glVertex2f(left + width, top);
			glVertex2f(left + width, top + height);
			glVertex2f(left, top + height);
		}
		glEnd();
	}

	void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float lineWidth)
	{
		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
			glVertex2f(x3, y3);
		}
		glEnd();
	}

	void DrawTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3, float lineWidth)
	{
		DrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, lineWidth);
	}

	void FillTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
	{
		glBegin(GL_TRIANGLES);
		{
			glVertex2f(x1, y1);
			glVertex2f(x2, y2);
			glVertex2f(x3, y3);
		}
		glEnd();
	}

	void FillTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3)
	{
		FillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
	}

	void DrawRect(const Point2f& topLeft, float width, float height, float lineWidth)
	{
		DrawRect(topLeft.x, topLeft.y, width, height, lineWidth);
	}

	void DrawRect(const Rectf& rect, float lineWidth)
	{
		DrawRect(rect.left, rect.top, rect.width, rect.height, lineWidth);
	}

	void FillRect(float left, float top, float width, float height)
	{
		glBegin(GL_POLYGON);
		{
			glVertex2f(left, top);
			glVertex2f(left + width, top);
			glVertex2f(left + width, top + height);
			glVertex2f(left, top + height);
		}
		glEnd();
	}

	void FillRect(const Point2f& topLeft, float width, float height)
	{
		FillRect(topLeft.x, topLeft.y, width, height);
	}

	void FillRect(const Rectf& rect)
	{
		FillRect(rect.left, rect.top, rect.width, rect.height);
	}

	void DrawEllipse(float centerX, float centerY, float radX, float radY, float lineWidth)
	{
		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			for (float angle = 0.0; angle < float(2 * g_Pi); angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
		}
		glEnd();
	}

	void DrawEllipse(const Point2f& center, float radX, float radY, float lineWidth)
	{
		DrawEllipse(center.x, center.y, radX, radY, lineWidth);
	}

	void DrawEllipse(const Ellipsef& ellipse, float lineWidth)
	{
		DrawEllipse(ellipse.center.x, ellipse.center.y, ellipse.radiusX, ellipse.radiusY, lineWidth);
	}

	void FillEllipse(float centerX, float centerY, float radX, float radY)
	{
		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glBegin(GL_POLYGON);
		{
			for (float angle = 0.0; angle < float(2 * g_Pi); angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
		}
		glEnd();
	}

	void FillEllipse(const Ellipsef& ellipse)
	{
		FillEllipse(ellipse.center.x, ellipse.center.y, ellipse.radiusX, ellipse.radiusY);
	}

	void FillEllipse(const Point2f& center, float radX, float radY)
	{
		FillEllipse(center.x, center.y, radX, radY);
	}

	void DrawArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle, float lineWidth)
	{
		if (fromAngle > tillAngle)
		{
			return;
		}

		//flip the angles (because view is flipped)
		fromAngle *= -1;
		tillAngle *= -1;
		std::swap(fromAngle, tillAngle); //from becomes to and vice versa

		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glLineWidth(lineWidth);
		glBegin(GL_LINE_STRIP);
		{
			for (float angle = fromAngle; angle < tillAngle; angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
			glVertex2f(centerX + radX * float(cos(tillAngle)), centerY + radY * float(sin(tillAngle)));
		}
		glEnd();

	}

	void DrawArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle, float lineWidth)
	{
		DrawArc(center.x, center.y, radX, radY, fromAngle, tillAngle, lineWidth);
	}

	void FillArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle)
	{
		if (fromAngle > tillAngle)
		{
			return;
		}

		//flip the angles (because view is flipped)
		fromAngle *= -1;
		tillAngle *= -1;
		std::swap(fromAngle, tillAngle); //from becomes to and vice versa

		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glBegin(GL_POLYGON);
		{
			glVertex2f(centerX, centerY);
			for (float angle = fromAngle; angle < tillAngle; angle += dAngle)
			{
				glVertex2f(centerX + radX * float(cos(angle)), centerY + radY * float(sin(angle)));
			}
			glVertex2f(centerX + radX * float(cos(tillAngle)), centerY + radY * float(sin(tillAngle)));
		}
		glEnd();
	}

	void FillArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle)
	{
		FillArc(center.x, center.y, radX, radY, fromAngle, tillAngle);
	}

	void DrawPolygon(const std::vector<Point2f>& vertices, bool closed, float lineWidth)
	{
		DrawPolygon(vertices.data(), vertices.size(), closed, lineWidth);
	}

	void DrawPolygon(const Point2f* pVertices, size_t nrVertices, bool closed, float lineWidth)
	{
		glLineWidth(lineWidth);
		closed ? glBegin(GL_LINE_LOOP) : glBegin(GL_LINE_STRIP);
		{
			for (size_t idx{ 0 }; idx < nrVertices; ++idx)
			{
				glVertex2f(pVertices[idx].x, pVertices[idx].y);
			}
		}
		glEnd();
	}

	void FillPolygon(const std::vector<Point2f>& vertices)
	{
		FillPolygon(vertices.data(), vertices.size());
	}

	void FillPolygon(const Point2f* pVertices, size_t nrVertices)
	{
		glBegin(GL_POLYGON);
		{
			for (size_t idx{ 0 }; idx < nrVertices; ++idx)
			{
				glVertex2f(pVertices[idx].x, pVertices[idx].y);
			}
		}
		glEnd();
	}
#pragma endregion OpenGLDrawFunctionality


#pragma region textureImplementations

	bool TextureFromFile(const std::string& path, Texture& texture)
	{
		//Load file for use as an image in a new surface.
		SDL_Surface* pLoadedSurface = IMG_Load((path).c_str());
		if (pLoadedSurface == nullptr)
		{
			std::cerr << "TextureFromFile: SDL Error when calling IMG_Load: " << SDL_GetError() << std::endl;
			return false;
		}

		TextureFromSurface(pLoadedSurface, texture);

		//Free loaded surface
		SDL_FreeSurface(pLoadedSurface);

		return true;
	}

	bool TextureFromString(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor, Texture& texture)
	{
		// Create font
		TTF_Font* pFont{};
		pFont = TTF_OpenFont((fontPath).c_str(), ptSize);
		if (pFont == nullptr)
		{
			std::cout << "TextureFromString: Failed to load font! SDL_ttf Error: " << TTF_GetError();
			std::cin.get();
			return false;
		}

		// Create texture using this fontand close font afterwards
		bool textureOk = TextureFromString(text, pFont, textColor, texture);
		TTF_CloseFont(pFont);

		return textureOk;
	}

	bool TextureFromString(const std::string& text, TTF_Font* pFont, const Color4f& color, Texture& texture)
	{
		if (pFont == nullptr)
		{
			std::cerr << "Texture::CreateFromString, invalid TTF_Font pointer\n";
			return false;
		}

		//Render text surface
		SDL_Color textColor{};
		textColor.r = Uint8(color.r * 255);
		textColor.g = Uint8(color.g * 255);
		textColor.b = Uint8(color.b * 255);
		textColor.a = Uint8(color.a * 255);

		SDL_Surface* pLoadedSurface = TTF_RenderText_Blended(pFont, text.c_str(), textColor);
		//SDL_Surface* pLoadedSurface = TTF_RenderText_Solid(pFont, textureText.c_str(), textColor);
		if (pLoadedSurface == nullptr)
		{
			std::cerr << "TextureFromString: Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << '\n';
			return false;
		}

		// copy to video memory
		TextureFromSurface(pLoadedSurface, texture);

		//Free loaded surface
		SDL_FreeSurface(pLoadedSurface);

		return true;
	}

	void TextureFromSurface(const SDL_Surface* pSurface, Texture& texture)
	{
		//Get image dimensions
		texture.width = float(pSurface->w);
		texture.height = float(pSurface->h);

		// Get pixel format information and translate to OpenGl format
		GLenum pixelFormat{ GL_RGB };
		switch (pSurface->format->BytesPerPixel)
		{
		case 3:
			if (pSurface->format->Rmask == 0x000000ff)
			{
				pixelFormat = GL_RGB;
			}
			else
			{
				pixelFormat = GL_BGR;
			}
			break;
		case 4:
			if (pSurface->format->Rmask == 0x000000ff)
			{
				pixelFormat = GL_RGBA;
			}
			else
			{
				pixelFormat = GL_BGRA;
			}
			break;
		default:
			std::cerr << "TextureFromSurface error: Unknow pixel format, BytesPerPixel: " << pSurface->format->BytesPerPixel << "\nUse 32 bit or 24 bit images.\n";;
			texture.width = 0;
			texture.height = 0;
			return;
		}

		//Generate an array of textures.  We only want one texture (one element array), so trick
		//it by treating "texture" as array of length one.
		glGenTextures(1, &texture.id);

		//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
		//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, pSurface->pitch / pSurface->format->BytesPerPixel);

		// check for errors.
		GLenum e = glGetError();
		if (e != GL_NO_ERROR)
		{
			std::cerr << "TextureFromSurface, error binding textures, Error id = " << e << '\n';
			std::cerr << "Can happen if a texture is created before performing the initialization code (e.g. a static Texture object).\n";
			std::cerr << "There might be a white rectangle instead of the image.\n";
			texture.width = 0;
			texture.height = 0;
			return;
		}

		//Specify the texture's data.  This function is a bit tricky, and it's hard to find helpful documentation.  A summary:
		//   GL_TEXTURE_2D:    The currently bound 2D texture (i.e. the one we just made)
		//               0:    The mipmap level.  0, since we want to update the base level mipmap image (i.e., the image itself,
		//                         not cached smaller copies)
		//         GL_RGBA:    Specifies the number of color components in the texture.
		//                     This is how OpenGL will store the texture internally (kinda)--
		//                     It's essentially the texture's type.
		//      surface->w:    The width of the texture
		//      surface->h:    The height of the texture
		//               0:    The border.  Don't worry about this if you're just starting.
		//     pixelFormat:    The format that the *data* is in--NOT the texture! 
		//GL_UNSIGNED_BYTE:    The type the data is in.  In SDL, the data is stored as an array of bytes, with each channel
		//                         getting one byte.  This is fairly typical--it means that the image can store, for each channel,
		//                         any value that fits in one byte (so 0 through 255).  These values are to be interpreted as
		//                         *unsigned* values (since 0x00 should be dark and 0xFF should be bright).
		// surface->pixels:    The actual data.  As above, SDL's array of bytes.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, pSurface->pixels);

		//Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
		//*smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
		//each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
		//further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
		//them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	void DeleteTexture(Texture& texture)
	{
		glDeleteTextures(1, &texture.id);
	}

	void DrawTexture(const Texture& texture, const Point2f& dstTopLeft, const Rectf& srcRect)
	{
		Rectf dstRect{ dstTopLeft.x, dstTopLeft.y, srcRect.width, srcRect.height };
		DrawTexture(texture, dstRect, srcRect);
	}


	void DrawTexture(const Texture& texture, const Rectf& dstRect, const Rectf& srcRect)
	{
		// Determine texture coordinates using srcRect and default destination width and height
		float textLeft{};
		float textRight{};
		float textTop{};
		float textBottom{};

		float defaultDstWidth{};
		float defaultDstHeight{};
		if (!(srcRect.width > 0.0f && srcRect.height > 0.0f)) // No srcRect specified
		{
			// Use complete texture
			textLeft = 0.0f;
			textRight = 1.0f;
			textTop = 0.0f;
			textBottom = 1.0f;

			defaultDstHeight = texture.height;
			defaultDstWidth = texture.width;
		}
		else // srcRect specified
		{
			textLeft = srcRect.left / texture.width;
			textRight = (srcRect.left + srcRect.width) / texture.width;
			textTop = srcRect.top / texture.height;
			textBottom = (srcRect.top + srcRect.height) / texture.height;

			defaultDstHeight = srcRect.height;
			defaultDstWidth = srcRect.width;
		}

		// Determine vertex coordinates
		float vertexTop{ dstRect.top };
		float vertexLeft{ dstRect.left };
		float vertexBottom{};
		float vertexRight{};
		if (!(dstRect.width > 0.001f && dstRect.height > 0.001f)) // If no size specified use default size
		{
			vertexRight = vertexLeft + defaultDstWidth;
			vertexBottom = vertexTop + defaultDstHeight;
		}
		else
		{
			vertexRight = vertexLeft + dstRect.width;
			vertexBottom = vertexTop + dstRect.height;
		}

		// Tell opengl which texture we will use
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		// Draw
		glEnable(GL_TEXTURE_2D);
		{
			glBegin(GL_QUADS);
			{
				glTexCoord2f(textLeft, textBottom);
				glVertex2f(vertexLeft, vertexBottom);

				glTexCoord2f(textLeft, textTop);
				glVertex2f(vertexLeft, vertexTop);

				glTexCoord2f(textRight, textTop);
				glVertex2f(vertexRight, vertexTop);

				glTexCoord2f(textRight, textBottom);
				glVertex2f(vertexRight, vertexBottom);
			}
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);

	}
#pragma endregion textureImplementations

#pragma region 2DArrayFunctionality

	int GetCol(int index, int numCols)
	{
		int col{};
		col = index % numCols;
		return col;
	}

	int GetRow(int index, int numCols)
	{
		int row{};
		row = index / numCols;
		return row;
	}

	int GetIndex(int rowIdx, int colIdx, int nrCols)
	{
		int index{};
		index = rowIdx * nrCols + colIdx;
		return index;
	}

	void Print2DArray(const int* array, int size, int nrCols) {
		for (int index{}; index <= nrCols; ++index) {
			std::cout << "-";
		}
		std::cout << "\n";

		for (int index{}; index < size; ++index) {
			if (!(index % nrCols))
				std::cout << "|";
			std::cout << array[index] << "";
			if (!((index + 1) % nrCols))
				std::cout << "|\n";
		}

		for (int index{}; index <= nrCols; ++index) {
			std::cout << "-";
		}
		std::cout << "\n";
	}

	void Print2DArray(const char* array, int size, int nrCols) {
		for (int index{}; index <= nrCols; ++index) {
			std::cout << "-";
		}
		std::cout << "\n";

		for (int index{}; index < size; ++index) {
			if (!(index % nrCols))
				std::cout << "|";
			std::cout << array[index] << "";
			if (!((index + 1) % nrCols))
				std::cout << "|\n";
		}

		for (int index{}; index <= nrCols; ++index) {
			std::cout << "-";
		}
		std::cout << "\n";
	}

#pragma endregion 2DArrayFunctionality



#pragma region CollisionFunctionality

	float	GetDistance(const Point2f& p1, const Point2f& p2) {
		const float
			dX{ fabsf(p2.x - p1.x) },
			dY{ fabsf(p2.y - p1.y) };

		return sqrt(dX * dX + dY * dY);
	}

	float	GetDistance(float x1, float y1, float x2, float y2) {
		const float
			dX{ fabsf(x2 - x1) },
			dY{ fabsf(y2 - y1) };

		return sqrt(dX * dX + dY * dY);
	}

	float	GetDistance(float p1, float p2) {
		return fabsf(p2 - p1);
	}

	bool	IsPointInCircle(const Point2f& point, const Circlef& circle) {
		return IsPointInCircle(point.x, point.y, circle.center.x, circle.center.y, circle.radius);
	}

	bool	IsPointInCircle(float x, float y, const Circlef& circle) {
		return IsPointInCircle(x, y, circle.center.x, circle.center.y, circle.radius);
	}

	bool	IsPointInCircle(const Point2f& point, const Point2f& center, float radius) {
		return IsPointInCircle(point.x, point.y, center.x, center.y, radius);
	}

	bool	IsPointInCircle(float x, float y, float centerX, float centerY, float radius) {
		const float dist{ GetDistance(x, y, centerX, centerY) };

		return (dist <= radius);
	}


	bool	IsPointInRect(const Point2f& point, const Rectf& rect) {
		return IsPointInRect(point.x, point.y, rect.left, rect.top, rect.width, rect.height);
	}

	bool	IsPointInRect(float x, float y, const Rectf& rect) {
		return IsPointInRect(x, y, rect.left, rect.top, rect.width, rect.height);
	}

	bool	IsPointInRect(float x, float y, float left, float top, float width, float height) {
		const float dX{ x - left }, dY{ y - top };

		if (dX < 0 || dY < 0)
			return false;
		if (dX > width || dY > height)
			return false;
		return true;
	}

	bool	IsOverlapping(const Circlef& c1, const Circlef& c2) {
		const float dist{ GetDistance(c1.center, c2.center) };
		return dist <= c1.radius + c2.radius;
	}

	bool	IsOverlapping(const Rectf& r1, const Rectf& r2) {
		bool isOverlapping{ true };

		if (r1.left > r2.left + r2.width || r2.left > r1.left + r1.width)
			isOverlapping = false;
		if (r1.top > r2.top + r2.height || r2.top > r1.top + r1.height)
			isOverlapping = false;

		return isOverlapping;
	}


#pragma endregion CollisionFunctionality

#pragma region Vector2fFunctionality

	void DrawVector(const Vector2f& vector, const Point2f& start) {

		const float angle{ g_Pi - atan2(vector.y, vector.x) },
			arrowLength{ 15.f };
		const Point2f end{ start.x + vector.x, start.y + vector.y };

		DrawLine(start, end);
		FillTriangle(end,
			Point2f{ end.x + cosf(angle + g_Pi / 6) * arrowLength , end.y - sinf(angle + g_Pi / 6) * arrowLength },
			Point2f{ end.x + cosf(angle - g_Pi / 6) * arrowLength , end.y - sinf(angle - g_Pi / 6) * arrowLength }
		);
	}

	std::string	ToString(const Vector2f& vector) {
		return ("[" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + "]");
	}

	Vector2f Add(const Vector2f& a, const Vector2f& b) {
		return Vector2f{ a.x + b.x, a.y + b.y };
	}

	Vector2f Substract(const Vector2f& a, const Vector2f& b) {
		return Vector2f{ a.x - b.x, a.y - b.y };
	}

	float DotProduct(const Vector2f& a, const Vector2f& b) {
		return a.x * b.x + a.y * b.y;
	}

	float CrossProduct(const Vector2f& a, const Vector2f& b) {
		/*	const float
				lengthA{ sqrt(a.x * a.x + a.y * a.y) },
				lengthB{ sqrt(b.x * b.x + b.y * b.y) },
				angle{ atan2(b.y, b.x) - atan2(a.y, a.x) };

			return lengthA * lengthB * sinf(angle);*/
		return (a.x * b.y - a.y * b.x);
	}

	float Length(const Vector2f& vector) {
		return sqrt(vector.x * vector.x + vector.y * vector.y);
	}

	Vector2f Scale(const Vector2f& vector, float scalar) {
		return Vector2f{ vector.x * scalar, vector.y * scalar };
	}

	Vector2f Normalize(const Vector2f& vector) {
		const float length{ Length(vector) };

		return Vector2f{ vector.x / length, vector.y / length };
	}

	float AngleBetween(const Vector2f& a, const Vector2f& b) {
		return atan2(b.y, b.x) - atan2(a.y, a.x);
	}

	bool AreEqual(const Vector2f& a, const Vector2f& b) {
		const Vector2f vZero{ 0.f, 0.f };
		return (Length(a) == Length(b) && AngleBetween(vZero, a) == AngleBetween(vZero, b));
	}

#pragma endregion Vector2fFunctionality


}