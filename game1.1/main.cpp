#include <SFML/Graphics.hpp>

using namespace sf;

const int H = 15;
const int W = 15;

const int blockSize = 50;

int Sti = 0;
bool shot = false;

String TileMap[H] = {
    " AAAAA11AAAA77 ",
    "B             E",
    "B             E",
    "2             E",
    "2             4",
    "B             E",
    "B             E",
    "5             E",
    "B             E",
    "B             E",
    "B             E",
    "B             E",
    "B             E",
    "B             E",
    " CCCCC33CCCCCC ",
};

class Player
{
  public:
    bool left, right, up, down;
    float dx, dy;
    int rotate;
    FloatRect rect;
    Sprite sprite;

    Player(Texture &image)
    {
        sprite.setTexture(image);
        rect = FloatRect(blockSize, blockSize * 8, 50, 50);
        sprite.setTextureRect(IntRect(0, 0, 50, 50));

        dx = dy = 0;
        down = true;
        up = left = right = false;
    }

    void update(float time)
    {
        rect.left += dx * time;
        collision(0);

        rect.top += dy * time;
        collision(1);

        if (left)
            rotate = 0;
        else if (right)
            rotate = 1;
        else if (up)
            rotate = 2;
        else if (down)
            rotate = 3;

        if (dx != 0 || dy != 0)
            sprite.setTextureRect(IntRect(200, 0, blockSize, blockSize));
        else if (dx == 0 && dy == 0)
            sprite.setTextureRect(IntRect(blockSize * rotate, 0, blockSize, blockSize));

        sprite.setPosition(rect.left, rect.top);
    }

    void collision(int direction)
    {
        for (int i = rect.top / blockSize; i < (rect.top + rect.height) / blockSize; i++)
            for (int j = rect.left / blockSize; j < (rect.left + rect.width) / blockSize; j++)
            {
                if (TileMap[i][j] == 'A' || TileMap[i][j] == 'B' || TileMap[i][j] == 'C' || TileMap[i][j] == 'E' || TileMap[i][j] == 'F' || TileMap[i][j] == '7')
                {
                    if (dx > 0 && direction == 0)
                    {
                        rect.left = j * blockSize - rect.width;
                        dx = 0;
                    }
                    if (dx < 0 && direction == 0)
                    {
                        rect.left = j * blockSize + blockSize;
                        dx = 0;
                    }
                    if (dy > 0 && direction == 1)
                    {
                        rect.top = i * blockSize - rect.height;
                        dy = 0;
                    }
                    if (dy < 0 && direction == 1)
                    {
                        rect.top = i * blockSize + blockSize;
                        dy = 0;
                    }
                }
            }
    }
};

class Bullet
{
  public:
    float dx, dy;
    int px, py, rotate, timer, Sti;
    bool pusk, shot;
    FloatRect rect;
    Sprite sprite;

    void set(Texture &image, int x, int y, int r)
    {
        sprite.setTexture(image);
        rect = FloatRect(x, y, 50, 50);
        sprite.setTextureRect(IntRect(blockSize * (r - 1), 0, 50, 50));

        if (r == 1)
        {
            dx = 0.5;
            dy = 0;
        }
        else if (r == 2)
        {
            dx = -0.5;
            dy = 0;
        }
        else if (r == 3)
        {
            dx = 0;
            dy = 0.5;
        }
        else if (r == 4)
        {
            dx = 0;
            dy = -0.5;
        }

        px = x;
        py = y;
        rotate = r;

        timer = 2000;
        Sti = 0;
        pusk = shot = false;
    }

    void update(float time)
    {
        if (timer > 0 && !pusk)
        {
            timer--;

            if (timer == 0)
            {
                timer = 2000;
                pusk = true;

                shot = true;
                Sti = 80;
            }
        }

        if (shot && Sti > 0)
        {
            Sti--;

            if (Sti == 0)
                shot = false;
        }

        if (pusk)
        {
            rect.left += dx * time;
            collision(0);

            rect.top += dy * time;
            collision(1);
        }

        sprite.setPosition(rect.left, rect.top);
    }

    void collision(int direction)
    {
        for (int i = rect.top / blockSize; i < (rect.top + rect.height) / blockSize; i++)
            for (int j = rect.left / blockSize; j < (rect.left + rect.width) / blockSize; j++)
            {
                if (TileMap[i][j] == 'A' || TileMap[i][j] == 'B' || TileMap[i][j] == 'C' ||
                    TileMap[i][j] == 'E' || TileMap[i][j] == 'F' || TileMap[i][j] == '1' || TileMap[i][j] == '7')
                {
                    if (dx != 0 && direction == 0)
                    {
                        rect.left = px;
                        pusk = false;
                    }
                    if (dy != 0 && direction == 1)
                    {
                        rect.top = py;
                        pusk = false;
                    }
                }
            }
    }

    void initBullets(Bullet *bullet, Texture &bulletTexture)
    {
        bullet[0].set(bulletTexture, blockSize, blockSize * 6, 1);
        bullet[1].set(bulletTexture, blockSize * 11, blockSize, 3);
    }
};

class Box
{
  public:
    Sprite sprite;
    int timer = 4000;
    bool big = false;

    void set(Texture &image, int x, int y)
    {
        sprite.setTexture(image);
        sprite.setPosition(x, y);
    }

    void update()
    {
        timer--;

        if (timer == 0)
        {
            if (big == 0)
            {
                big = true;
                timer = 2000;
            }
            else
            {
                big = false;
                timer = 4000;
            }
        }

        sprite.setTextureRect(IntRect(150 * big, 0, 150, 150));
    }
    void initBoxes(Box *box, Texture &boxTexture)
    {
        box[0].set(boxTexture, blockSize * 7, blockSize);
    }
};

class Bat
{
  public:
    Sprite sprite;
    FloatRect rect;
    float curFrame, dx, dy;

    void set(Texture &image, int x, int y, bool Cx)
    {
        sprite.setTexture(image);
        rect = FloatRect(x, y, 50, 50);

        curFrame = 0;

        if (Cx)
        {
            dx = -0.1;
            dy = 0;
        }
        else
        {
            dx = 0;
            dy = -0.1;
        }
    }

    void update(float time)
    {
        rect.left += dx * time;

        collision(0);

        rect.top += dy * time;

        collision(1);

        curFrame += 0.005 * time;
        if (curFrame > 3)
            curFrame -= 3;

        if (dx < 0 || dy > 0)
            sprite.setTextureRect(IntRect(50 * int(curFrame), 0, 50, 50));
        else
            sprite.setTextureRect(IntRect(50 * int(curFrame) + 50, 0, -50, 50));

        sprite.setPosition(rect.left, rect.top);
    }

    void collision(int direction)
    {
        for (int i = rect.top / blockSize; i < (rect.top + rect.height) / blockSize; i++)
            for (int j = rect.left / blockSize; j < (rect.left + rect.width) / blockSize; j++)
                if (TileMap[i][j] == 'A' || TileMap[i][j] == 'B' || TileMap[i][j] == 'C' ||
                    TileMap[i][j] == 'E' || TileMap[i][j] == 'F' || TileMap[i][j] == '7')
                {
                    if (dx != 0 && direction == 0)
                    {
                        dx *= -1;
                    }
                    else if (dy != 0 && direction == 1)
                    {
                        dy *= -1;
                    }
                }
    }
    void initBats(Bat *bat, Texture &batTexture)
    {
        bat[0].set(batTexture, blockSize * 12, blockSize * 5, true);
        bat[1].set(batTexture, blockSize * 4, blockSize * 4, false);
    }
};

// class InitMobs
// {}

// void initLevel() {

// }

void texturesTreatment()
{
    RenderWindow window(VideoMode(750, 750), "tomb of the mask");

    Texture titleTexture;
    titleTexture.loadFromFile("additional/Paint/title.png");
    Sprite title(titleTexture);

    Texture playerTexture;
    playerTexture.loadFromFile("additional/Paint/player.png");
    Player player(playerTexture);

    Texture bulletTexture;
    bulletTexture.loadFromFile("additional/Paint/bullet.png");
    Bullet bullet[4];
    bullet[0].initBullets(bullet, bulletTexture);

    Texture boxTexture;
    boxTexture.loadFromFile("additional/Paint/box.png");
    Box box[2];
    box[0].initBoxes(box, boxTexture);

    Texture batTexture;
    batTexture.loadFromFile("additional/Paint/bat.png");
    Bat bat[2];
    bat[0].initBats(bat, batTexture);

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed && player.dx == 0 && player.dy == 0)
            {
                if (event.key.code == Keyboard::Right)
                {
                    player.dx = 1;
                    player.left = player.up = player.down = false;
                    player.right = true;
                }
                if (event.key.code == Keyboard::Left)
                {
                    player.dx = -1;
                    player.right = player.up = player.down = false;
                    player.left = true;
                }
                if (event.key.code == Keyboard::Up)
                {
                    player.dy = -1;
                    player.right = player.left = player.down = false;
                    player.up = true;
                }
                if (event.key.code == Keyboard::Down)
                {
                    player.dy = 1;
                    player.right = player.left = player.up = false;
                    player.down = true;
                }
            }
        }

        player.update(time);
        for (int i = 0; i < 2; i++)
            box[i].update();
        for (int i = 0; i < 2; i++)
            bat[i].update(time);
        for (int i = 0; i < 4; i++)
            bullet[i].update(time);
        window.clear(Color::Black);
        for (int i = 0; i < H; i++)
            for (int j = 0; j < W; j++)
            {
                if (TileMap[i][j] == 'A')
                    title.setTextureRect(IntRect(0, 0, blockSize, blockSize));
                if (TileMap[i][j] == 'B')
                    title.setTextureRect(IntRect(blockSize, 0, blockSize, blockSize));
                if (TileMap[i][j] == 'C')
                    title.setTextureRect(IntRect(blockSize * 2, 0, blockSize, blockSize));
                if (TileMap[i][j] == 'E')
                    title.setTextureRect(IntRect(blockSize * 3, 0, blockSize, blockSize));
                if (TileMap[i][j] == '1')
                    title.setTextureRect(IntRect(0, blockSize, blockSize, blockSize));
                if (TileMap[i][j] == '2')
                    title.setTextureRect(IntRect(blockSize, blockSize, blockSize, blockSize));
                if (TileMap[i][j] == '3')
                    title.setTextureRect(IntRect(blockSize * 2, blockSize, blockSize, blockSize));
                if (TileMap[i][j] == '4')
                    title.setTextureRect(IntRect(blockSize * 3, blockSize, blockSize, blockSize));
                if (TileMap[i][j] == '5')
                    title.setTextureRect(IntRect(blockSize * 4, blockSize * bullet[0].shot, blockSize, blockSize));
                if (TileMap[i][j] == '6')
                    title.setTextureRect(IntRect(blockSize * 5, blockSize * bullet[1].shot, blockSize, blockSize));
                if (TileMap[i][j] == '7')
                    title.setTextureRect(IntRect(blockSize * 6, blockSize * bullet[2].shot, blockSize, blockSize));
                if (TileMap[i][j] == '8')
                    title.setTextureRect(IntRect(blockSize * 7, blockSize * bullet[3].shot, blockSize, blockSize));
                if (TileMap[i][j] == ' ')
                    continue;

                title.setPosition(j * blockSize, i * blockSize);
                window.draw(title);
            }
        window.draw(player.sprite);
        for (int i = 0; i < 2; i++)
            window.draw(box[i].sprite);
        for (int i = 0; i < 2; i++)
            window.draw(bat[i].sprite);
        for (int i = 0; i < 4; i++)
            if (bullet[i].pusk)
                window.draw(bullet[i].sprite);
        window.display();
    }
}

int main()
{
    texturesTreatment();

    return 0;
}