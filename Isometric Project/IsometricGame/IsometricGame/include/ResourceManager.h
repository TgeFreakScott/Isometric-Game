#ifndef RESOURCE_MNG
#define RESOURCE_MNG

#include "Thor\Resources.hpp"
#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
#include <iostream>


// How-To Resource manager:
//		1. Add new ID for the new file in the corresponding enum
//		2. Load the file in the corresponding function
//		3. Use the newly made ID for that texture 
//		4. You can now use the texture in your class

// Enum class used when loading textures and getting a texture
enum class TextureID
{
	BACKGROUND
};

// Enum class used when loading fonts and getting a font
enum class FontID
{
	ARIBLK,
	ORDINARY
};


// Enum class used when loading sound buffers and getting a buffer
enum class SoundBufferID
{

};

// class that holds and handles loading different resources
class ResourceManager
{
public:
	ResourceManager();  // default constructor
	~ResourceManager(); // default destructor

	// get functions for different holders

	// gets a texture from the holder
	sf::Texture & getTexture(TextureID t_textureID);
	
	// gets a font from the holder
	sf::Font & getFont(FontID t_fontID);
	
	// gets a sound buffer
	sf::SoundBuffer & getBuffer(SoundBufferID t_bufferID);

private:
	// functions
	void loadTextures();  // loads textures into the fontHolder
	void loadFonts();	  // loads fonts into fontHolder
	void loadBuffers();	  // loads audio files into the bufferHolder

	// variables
	thor::ResourceHolder<sf::Texture, TextureID> m_textureHolder;
	thor::ResourceHolder<sf::Font, FontID> m_fontHolder;
	thor::ResourceHolder<sf::SoundBuffer, SoundBufferID> m_bufferHolder;
};

#endif // !RESOURCE_MNG