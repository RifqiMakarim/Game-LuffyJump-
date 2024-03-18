#include <SFML\Graphics.hpp>
#include <vector>
#include <random>
#include <ctime>   

void showMainMenu(sf::RenderWindow& window) {
	sf::Font Start,title;
	Start.loadFromFile("font/RESEARCH AND DEVELOPMENT.otf");
	title.loadFromFile("font/Joorick.ttf");

	// Declare and load the texture
	sf::Texture startBackgroundTexture;
	startBackgroundTexture.loadFromFile("images/bg-start.jpg");
	sf::Sprite startBackgroundSprite(startBackgroundTexture);
	startBackgroundSprite.setPosition(0, 0);

	sf::Text titleText;
	titleText.setFont(title);
	titleText.setString("Luffy Jump!");
	titleText.setCharacterSize(60);
	titleText.setFillColor(sf::Color::Red);
	titleText.setPosition(100, 200);

	sf::Text startText;
	startText.setFont(Start);
	startText.setString("Press 'S' to Start");
	startText.setCharacterSize(20);
	startText.setFillColor(sf::Color::White);
	startText.setPosition(100, 300);

	window.clear(); //membersihkan window
	window.draw(startBackgroundSprite);
	window.draw(titleText);
	window.draw(startText);
	window.display(); // menampilkan object yang sebelumnya di-draw

	// Menekan Tombol "S" untuk memulai game
	while (true) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				exit(0);
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::S) {
					return;
				}
			}
		}
	}
}

class SpecialPlatform {
public:
	sf::Vector2f position;
	sf::Texture texture;
	sf::Sprite sprite;
	float jumpHeight;
	bool isUsed; // Menandakan apakah special platform sudah digunakan

	SpecialPlatform(sf::RenderWindow& window, float height)
		: jumpHeight(height), isUsed(false) {
		texture.loadFromFile("images/special_platform.png");
		sprite.setTexture(texture);

		// Set position at a random location along the X-axis
		std::uniform_int_distribution<unsigned> x(0, window.getSize().x - texture.getSize().x);
		std::default_random_engine e(time(0));
		position.x = static_cast<float>(x(e));

		// Set position above the regular platforms
		position.y = 600.0f;
		sprite.setPosition(position);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(sprite);
	}

	bool checkCollision(sf::FloatRect playerBounds) {
		return sprite.getGlobalBounds().intersects(playerBounds);
	}

	sf::Vector2f getPosition() const {
		return position;
	}
};

class BlockingObject {
public:
	sf::Vector2f position;
	sf::Texture texture;
	sf::Sprite sprite;
	bool isActive; // Menandakan blocking object aktif atau tidak aktif

	BlockingObject(sf::RenderWindow& window)
		: texture(), sprite(), isActive(false) {
		texture.loadFromFile("images/chibi_kaido.png");
		sprite.setTexture(texture);

		// Set the position above the special platform
		position.x = window.getSize().x / 2.0f - texture.getSize().x / 2.0f;
		position.y = 550.0f; 
		sprite.setPosition(position);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(sprite);
	}

	bool checkCollision(sf::FloatRect playerBounds) {
		return sprite.getGlobalBounds().intersects(playerBounds);
	}

	void setPosition(float x, float y) {
		position.x = x;
		position.y = y;
		sprite.setPosition(position);
	}

	void Active() {
		isActive = true;
	}

	void deactivate() {
		isActive = false;
	}

	bool getIsActive() const {
		return isActive;
	}
};

class FallingObject {
public:
	sf::Vector2f position;
	sf::Texture texture;
	sf::Sprite sprite;
	float speed;

	FallingObject(float fallSpeed)
		: speed(fallSpeed) {
		texture.loadFromFile("images/fireballs.png");  
		sprite.setTexture(texture);
		sprite.setPosition(getRandomX(), 0);
	}

	void update(float dt) {
		position.y += speed * dt;

		if (position.y > 700) {
			position.y = 0;
			position.x = getRandomX();
		}

		sprite.setPosition(position);
	}

	float getRandomX() {
		std::uniform_int_distribution<unsigned> x(0, 500 - texture.getSize().x);
		std::default_random_engine e(time(0));
		return static_cast<float>(x(e));
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 700), "Luffy Jump!", sf::Style::Close); 
	window.setFramerateLimit(60);

mainmenu :

	showMainMenu(window);

	sf::Texture backgroundTexture;
	sf::Texture playerTexture;
	sf::Texture platformTexture;
	backgroundTexture.loadFromFile("images/FloatingIsland.jpeg");
	playerTexture.loadFromFile("images/Luffy_Gear5th.png");
	platformTexture.loadFromFile("images/Awan.png");
	sf::Sprite background(backgroundTexture);
	sf::Sprite player(playerTexture);
	sf::Sprite platform(platformTexture);
	std::vector<SpecialPlatform> specialPlatforms;
	bool displaySpecialPlatform = false;

	// Jumlah loncatan khusus yang diperoleh setelah menggunakan special platform
	int specialPlatformJumps = 2;

	// Counter untuk menghitung berapa kali player telah melakukan loncatan khusus
	int jumpsRemaining = 0;

	BlockingObject blockingObject(window);
	bool playerOnSpecialPlatform = false;

	// Inisialisasi objek jatuh
	FallingObject fallingObject(300.0f);
	sf::Clock clock;
	bool gameOver = false;

	//Mengganti Jenis Font dalam Game
	sf::Font super_peach,watermark_font,Restart_Font;
	super_peach.loadFromFile("font/Super Peach.ttf");
	watermark_font.loadFromFile("font/Darwin Smith.otf");
	Restart_Font.loadFromFile("font/RESEARCH AND DEVELOPMENT.otf");

	sf::Text scoreText;
	scoreText.setFont(super_peach);
	scoreText.setCharacterSize(40);
	scoreText.setFillColor(sf::Color::Red);

	sf::RectangleShape gameoverBackground(sf::Vector2f(500, 700));
	gameoverBackground.setFillColor(sf::Color::White);
	sf::Text gameoverText;
	gameoverText.setFont(super_peach);
	gameoverText.setString("Game Over!");
	gameoverText.setCharacterSize(75);
	gameoverText.setFillColor(sf::Color::Red);

	// initialize platforms
	sf::Vector2u platformPosition[10];
	std::uniform_int_distribution<unsigned> x(0, 500 - platformTexture.getSize().x);
	std::uniform_int_distribution<unsigned> y(100, 700);
	std::default_random_engine e(time(0));
	for (size_t i = 0; i < 10; ++i)
	{
		platformPosition[i].x = x(e);
		platformPosition[i].y = y(e);
	}

	// player's positon and down velocity
	int playerX = 250;
	int playerY = 151;
	float dy = 0;
	int height = 150;
	int score = 0;

	// player's bounding box. It should modify according to the image size
	const int PLAYER_LEFT_BOUNDING_BOX = 20;
	const int PLAYER_RIGHT_BOUNDING_BOX = 60;
	const int PLAYER_BOTTOM_BOUNDING_BOX = 70;

	//restart 
	sf::Text restartText;
	restartText.setFont(Restart_Font);
	restartText.setString("Press 'R' to Restart");
	restartText.setCharacterSize(10);
	restartText.setFillColor(sf::Color::Black);
	restartText.setPosition(150, 500);

	//watermark
	sf::Text watermark;
	watermark.setFont(watermark_font);
	watermark.setString("Rifqi Makarim\nL0123122");
	watermark.setCharacterSize(25);
	watermark.setFillColor(sf::Color::Black);
	watermark.setPosition(30, 600);

	while (window.isOpen() && !gameOver)
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			playerX -= 4;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			playerX += 4;

		//Mengatur agar player keluar kiri layar maka akan masuk ke kanan layar begitupun sebalinya
		if (playerX > 500)
			playerX = 0;
		if (playerX < -40)
			playerX = window.getSize().x - playerTexture.getSize().x;

		if (playerY == height && dy < (-1.62))
		{
			score += 1;
			scoreText.setString("Score: " + std::to_string(score));
		}

		// Mengatur Lompatan Karakter player
		dy += 0.2;
		playerY += dy;

		if (playerY < height)
			for (size_t i = 0; i < 10; ++i)
			{
				playerY = height;
				platformPosition[i].y -= dy;  // vertical translation
				if (platformPosition[i].y > 700) // set new platform on the top
				{
					platformPosition[i].y = 0;
					platformPosition[i].x = x(e);
				}
			}

		for (size_t i = 0; i < 10; ++i)
		{
			if ((playerX + PLAYER_RIGHT_BOUNDING_BOX > platformPosition[i].x) && (playerX + PLAYER_LEFT_BOUNDING_BOX < platformPosition[i].x + platformTexture.getSize().x)        // player's horizontal range can touch the platform
				&& (playerY + PLAYER_BOTTOM_BOUNDING_BOX > platformPosition[i].y) && (playerY + PLAYER_BOTTOM_BOUNDING_BOX < platformPosition[i].y + platformTexture.getSize().y)  // player's vertical   range can touch the platform
				&& (dy > 0)) // player is falling
			{
				//sound.play();
				dy = -10;
			}
		}
		player.setPosition(playerX, playerY);

		window.draw(background);
		window.draw(player);

		// set and draw platforms
		for (size_t i = 0; i < 10; ++i)
		{
			platform.setPosition(platformPosition[i].x, platformPosition[i].y);
			window.draw(platform);
		}

		//Special Platform
		
		if (!displaySpecialPlatform) {
			displaySpecialPlatform = true;
			specialPlatforms.emplace_back(window, 100.0f);
		}

		// Update and draw special platforms
		for (auto& specialPlatform : specialPlatforms) {
			specialPlatform.draw(window);
			if (specialPlatform.checkCollision(player.getGlobalBounds()) && dy > 0 && !specialPlatform.isUsed) {
				// Efek loncatan khusus
				dy = -100.0;
				jumpsRemaining = specialPlatformJumps;  // Setel jumlah loncatan khusus yang tersisa

				// Set special platform sudah digunakan
				specialPlatform.isUsed = true;

				blockingObject.Active();
			}
		}

		// Kurangi jumlah loncatan khusus yang tersisa jika player sedang dalam efek loncatan khusus
		if (jumpsRemaining > 0) {
			jumpsRemaining--;
		}

		// Draw blocking object only when it is active
		if (blockingObject.getIsActive()) {
			blockingObject.draw(window);

			// Check for collision with blocking object
			if (blockingObject.checkCollision(player.getGlobalBounds())) {
				// Game over if player touches the blocking object
				gameOver = true;
			}
		}

		// update objek jatuh
		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();
		fallingObject.update(dt);
		window.draw(fallingObject.sprite);

		// Jika pemain bertabrakan dengan objek jatuh
		if ((playerX + PLAYER_RIGHT_BOUNDING_BOX > fallingObject.position.x) &&
			(playerX + PLAYER_LEFT_BOUNDING_BOX < fallingObject.position.x + fallingObject.texture.getSize().x) &&
			(playerY + PLAYER_BOTTOM_BOUNDING_BOX > fallingObject.position.y) &&
			(playerY + PLAYER_BOTTOM_BOUNDING_BOX < fallingObject.position.y + fallingObject.texture.getSize().y)) {
			// Game over
			gameOver = true;
		}
		
		// menentukan kapan objek jatuh muncul
		if (fallingObject.position.y > 700) {
			fallingObject.position.y = 0;
			fallingObject.position.x = fallingObject.getRandomX();
		}

		// game over
		if (playerY > 700)
		{
			scoreText.setPosition(150, 400);
			goto gameover;
		}
		window.draw(scoreText);
		window.display();
	}

	// Game Over
gameover:
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		gameoverText.setPosition(50, 200);
		scoreText.setPosition(150, 300);

		window.draw(gameoverBackground);
		window.draw(gameoverText);
		window.draw(scoreText);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			// Reset game variables
			playerX = 250;
			playerY = 151;
			dy = 0;
			height = 150;
			score = 0;
			jumpsRemaining = 0;

			// Reset special platforms
			for (auto& specialPlatform : specialPlatforms) {
				specialPlatform.isUsed = false;
			}

			// Reset blocking object
			blockingObject.deactivate();

			// Generate new special platforms
			displaySpecialPlatform = false;

			//generate new platform positions
			for (size_t i = 0; i < 10; ++i)
			{
				platformPosition[i].x = x(e);
				platformPosition[i].y = y(e);
			}

			gameOver = false;
			goto mainmenu;
		}

		window.draw(restartText);
		window.draw(watermark);

		window.display();
	}
	return 0;
}