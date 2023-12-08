#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

const int fieldHeight = 22;
const int fieldWidth = 25;

const int blockSize = 50;

float offsetX = -300;
float offsetY = 700;

bool game = true;
bool win = false;

String Field[fieldHeight] = {
    "       AAAAA7A AAAAAAAAA ",
    "      B       D         K",
    "      B GFH   I   J     K",
    "      B   D       D     K",
    "      B   EFF8FFFFB     K",
    "      B   D       D     K",
    "      B  WD       D     K",
    "       CCCB   J   D     K",
    "          B   D   KCCCH K",
    "          B   D  G    B K",
    "          5   D   6AAAL K",
    "          B   D   D     K",
    "          5   KM  D     K",
    "   AAAA AAL   D   D     K",
    "  B    I      D   D  GFF ",
    "  B J     NCCCB  GB     K",
    "  B D     K   B   6     K",
    " AL KCCCC3    B   KFFFM K",
    "B   K         B   I     K",
    "B   K         B         4",
    "B   K         2     J   4",
    " CCC           CCCCC CCC ",
};

class Player
{
  public:
    float dx, dy;
    FloatRect rect;
    Sprite sprite;
    bool left, right, up, down;
    int rotate;

    Player(Texture &image)
    {
        sprite.setTexture(image);
        rect = FloatRect(blockSize * 2, blockSize * 20, 50, 50);
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

        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
    }

    void collision(int dir)
    {
        for (int i = rect.top / blockSize; i < (rect.top + rect.height) / blockSize; i++)
            for (int j = rect.left / blockSize; j < (rect.left + rect.width) / blockSize; j++)
            {
                if (Field[i][j] == 'A' || Field[i][j] == 'B' || Field[i][j] == 'C' || Field[i][j] == 'D' ||
                    Field[i][j] == 'E' || Field[i][j] == 'F' || Field[i][j] == 'G' || Field[i][j] == 'H' ||
                    Field[i][j] == 'I' || Field[i][j] == 'J' || Field[i][j] == 'K' || Field[i][j] == 'L' ||
                    Field[i][j] == 'M' || Field[i][j] == 'N' ||
                    Field[i][j] == '5' || Field[i][j] == '6' || Field[i][j] == '7' || Field[i][j] == '8')
                {
                    if (dx > 0 && dir == 0)
                    {
                        rect.left = j * blockSize - rect.width;
                        dx = 0;
                    }
                    if (dx < 0 && dir == 0)
                    {
                        rect.left = j * blockSize + blockSize;
                        dx = 0;
                    }
                    if (dy > 0 && dir == 1)
                    {
                        rect.top = i * blockSize - rect.height;
                        dy = 0;
                    }
                    if (dy < 0 && dir == 1)
                    {
                        rect.top = i * blockSize + blockSize;
                        dy = 0;
                    }
                }

                if (Field[i][j] == '1' || Field[i][j] == '2' || Field[i][j] == '3' || Field[i][j] == '4')
                    game = false;
                if (Field[i][j] == 'W')
                {
                    win = true;
                }
            }
    }
};

class Bullet
{
  public:
    float dx, dy;
    FloatRect rect;
    Sprite sprite;
    int px, py, rotate, timer, Sti;
    bool pusk, shot;

    void set(Texture &image, int x, int y, int r)
    {
        sprite.setTexture(image);
        rect = FloatRect(x, y, 50, 50);
        sprite.setTextureRect(IntRect(blockSize * (r - 1), 0, 50, 50));

        if (r == 1)
        {
            dx = 0.2;
            dy = 0;
        }
        else if (r == 2)
        {
            dx = -0.2;
            dy = 0;
        }
        else if (r == 3)
        {
            dx = 0;
            dy = 0.2;
        }
        else if (r == 4)
        {
            dx = 0;
            dy = -0.2;
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
                timer = 1000;
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

        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
    }

    void collision(int dir)
    {
        for (int i = rect.top / blockSize; i < (rect.top + rect.height) / blockSize; i++)
            for (int j = rect.left / blockSize; j < (rect.left + rect.width) / blockSize; j++)
            {
                if (Field[i][j] == 'A' || Field[i][j] == 'B' || Field[i][j] == 'C' || Field[i][j] == 'D' ||
                    Field[i][j] == 'E' || Field[i][j] == 'F' || Field[i][j] == 'G' || Field[i][j] == 'H' ||
                    Field[i][j] == 'I' || Field[i][j] == 'J' || Field[i][j] == 'K' || Field[i][j] == 'L' ||
                    Field[i][j] == 'M' || Field[i][j] == 'N' ||
                    Field[i][j] == '5' || Field[i][j] == '6' || Field[i][j] == '7' || Field[i][j] == '8')
                {
                    if (dx != 0 && dir == 0)
                    {
                        rect.left = px;
                        pusk = false;
                    }
                    if (dy != 0 && dir == 1)
                    {
                        rect.top = py;
                        pusk = false;
                    }
                }
            }
    }

    void initBullet(Bullet *bullet, Texture &bulletTexture)
    {
        bullet[0].set(bulletTexture, blockSize * 11, blockSize * 12, 1);
        bullet[1].set(bulletTexture, blockSize * 11, blockSize * 10, 1);
        bullet[2].set(bulletTexture, blockSize * 17, blockSize * 10, 2);
        bullet[3].set(bulletTexture, blockSize * 17, blockSize * 16, 2);
        bullet[4].set(bulletTexture, blockSize * 12, blockSize * 1, 3);
        bullet[5].set(bulletTexture, blockSize * 13, blockSize * 3, 4);
    }
};

class Box
{
  public:
    Sprite sprite;
    int timer = 2000;
    bool big = 0;
    int Bx, By;
    FloatRect rect;

    void set(Texture &image, int x, int y)
    {
        sprite.setTexture(image);

        Bx = x;
        By = y;
    }

    void update()
    {
        timer--;

        if (timer == 0)
        {
            if (big == 0)
            {
                big = 150;
                timer = 1000;
            }
            else
            {
                big = 0;
                timer = 2000;
            }
        }

        rect.left = Bx;
        rect.top = By;

        sprite.setTextureRect(IntRect(150 * big, 0, 150, 150));
        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
    }

    void initBoxes(Box *box, Texture &boxTexture)
    {
        box[0].set(boxTexture, blockSize * 21, blockSize * 18);
        box[1].set(boxTexture, blockSize * 19, blockSize);
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

        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
    }

    void collision(int dir)
    {
        for (int i = rect.top / blockSize; i < (rect.top + rect.height) / blockSize; i++)
            for (int j = rect.left / blockSize; j < (rect.left + rect.width) / blockSize; j++)
                if (Field[i][j] == 'A' || Field[i][j] == 'B' || Field[i][j] == 'C' || Field[i][j] == 'D' ||
                    Field[i][j] == 'E' || Field[i][j] == 'F' || Field[i][j] == 'G' || Field[i][j] == 'H' ||
                    Field[i][j] == 'I' || Field[i][j] == 'J' || Field[i][j] == 'K' || Field[i][j] == 'L' ||
                    Field[i][j] == 'M' || Field[i][j] == 'N' ||
                    Field[i][j] == '5' || Field[i][j] == '6' || Field[i][j] == '7' || Field[i][j] == '8')
                {
                    if (dx > 0 && dir == 0)
                    {
                        dx *= -1;
                    }
                    else if (dx < 0 && dir == 0)
                    {
                        dx *= -1;
                    }
                    else if (dy < 0 && dir == 1)
                    {
                        dy *= -1;
                    }
                    else if (dy > 0 && dir == 1)
                    {
                        dy *= -1;
                    }
                }
    }

    void initBats(Bat *bat, Texture &batTexture)
    {
        bat[0].set(batTexture, blockSize * 8, blockSize * 15, false);
        bat[1].set(batTexture, blockSize * 16, blockSize * 7, true);
        bat[2].set(batTexture, blockSize * 16, blockSize * 13, true);
        bat[3].set(batTexture, blockSize * 20, blockSize * 12, false);
    }
};

void texturesTreatment()
{
    RenderWindow window(VideoMode(1200, 900), "tomb of the mask");
    Texture t;
    t.loadFromFile("additional/Paint/title.png");
    Sprite titleTexture(t);

    Texture Picture;
    Picture.loadFromFile("additional/Paint/player.png");
    Player playerTexture(Picture);

    Texture batTexture;
    batTexture.loadFromFile("additional/Paint/bat.png");
    Bat bat[4];
    bat[0].initBats(bat, batTexture);

    Texture boxTexture;
    boxTexture.loadFromFile("additional/Paint/box.png");
    Box box[2];
    box[0].initBoxes(box, boxTexture);

    Texture bulletTexture;
    bulletTexture.loadFromFile("additional/Paint/bullet.png");
    Bullet bullet[6];
    bullet[0].initBullet(bullet, bulletTexture);

    Clock clock;

    float isk = 100;

    Texture yl;
    yl.loadFromFile("additional/Paint/youlose.png");
    Sprite youlose(yl);
    youlose.setPosition(100, 210);

    Texture wn;
    wn.loadFromFile("additional/Paint/youwin.png");
    Sprite youwin(wn);
    youwin.setPosition(100, 350);

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;

        Vector2i pos = Mouse::getPosition(window);

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed && playerTexture.dx == 0 && playerTexture.dy == 0)
            {
                if (event.key.code == Keyboard::Right)
                {
                    playerTexture.dx = 1;

                    playerTexture.left = playerTexture.up = playerTexture.down = false;
                    playerTexture.right = true;
                }
                if (event.key.code == Keyboard::Left)
                {
                    playerTexture.dx = -1;

                    playerTexture.right = playerTexture.up = playerTexture.down = false;
                    playerTexture.left = true;
                }
                if (event.key.code == Keyboard::Up)
                {
                    playerTexture.dy = -1;

                    playerTexture.right = playerTexture.left = playerTexture.down = false;
                    playerTexture.up = true;
                }
                if (event.key.code == Keyboard::Down)
                {
                    playerTexture.dy = 1;

                    playerTexture.right = playerTexture.left = playerTexture.up = false;
                    playerTexture.down = true;
                }
            }
        }

        for (int i = 0; i < 4; i++)
            if (playerTexture.rect.intersects(bat[i].rect))
                game = false;

        for (int i = 0; i < 6; i++)
            if (bullet[i].pusk && playerTexture.rect.intersects(bullet[i].rect))
                game = false;

        for (int i = 0; i < 2; i++)
            if (box[i].big == 1 &&
                box[i].rect.left <= playerTexture.rect.left && playerTexture.rect.left <= box[i].rect.left + 2 * blockSize &&
                box[i].rect.top <= playerTexture.rect.top && playerTexture.rect.top <= box[i].rect.top + 2 * blockSize)
                game = false;

        if (playerTexture.rect.left > 250 && playerTexture.rect.left < 1000)
            offsetX = playerTexture.rect.left - 550;
        if (playerTexture.rect.top > 250 && playerTexture.rect.top < 1500)
            offsetY = playerTexture.rect.top - 350;

        if (!game /* && !sound.stop*/)
        {
            // sound.lose.play();
            // sound.stop = true;
        }

        if (game && !win)
        {
            for (int i = 0; i < 4; i++)
                bat[i].update(time);
            playerTexture.update(time);
            for (int i = 0; i < 2; i++)
                box[i].update();
            for (int i = 0; i < 6; i++)
                bullet[i].update(time);
        }
        window.clear(Color::Black);
        for (int i = 0; i < fieldHeight; i++)
            for (int j = 0; j < fieldWidth; j++)
            {
                if (Field[i][j] == 'A')
                    titleTexture.setTextureRect(IntRect(0, 0, blockSize, blockSize));
                if (Field[i][j] == 'B')
                    titleTexture.setTextureRect(IntRect(blockSize, 0, blockSize, blockSize));
                if (Field[i][j] == 'C')
                    titleTexture.setTextureRect(IntRect(blockSize * 2, 0, blockSize, blockSize));
                if (Field[i][j] == 'D')
                    titleTexture.setTextureRect(IntRect(0, blockSize * 2, blockSize, blockSize));
                if (Field[i][j] == 'E')
                    titleTexture.setTextureRect(IntRect(blockSize * 3, 0, blockSize, blockSize));
                if (Field[i][j] == 'F')
                    titleTexture.setTextureRect(IntRect(blockSize, blockSize * 2, blockSize, blockSize));
                if (Field[i][j] == 'G')
                    titleTexture.setTextureRect(IntRect(blockSize * 2, blockSize * 2, blockSize, blockSize));
                if (Field[i][j] == 'H')
                    titleTexture.setTextureRect(IntRect(blockSize * 3, blockSize * 2, blockSize, blockSize));
                if (Field[i][j] == 'I')
                    titleTexture.setTextureRect(IntRect(blockSize * 4, blockSize * 2, blockSize, blockSize));
                if (Field[i][j] == 'J')
                    titleTexture.setTextureRect(IntRect(blockSize * 5, blockSize * 2, blockSize, blockSize));
                if (Field[i][j] == 'K')
                    titleTexture.setTextureRect(IntRect(blockSize * 3, 0, blockSize, blockSize));
                if (Field[i][j] == 'L')
                    titleTexture.setTextureRect(IntRect(blockSize * 6, blockSize * 2, blockSize, blockSize));
                if (Field[i][j] == 'M')
                    titleTexture.setTextureRect(IntRect(blockSize * 7, blockSize * 2, blockSize, blockSize));
                if (Field[i][j] == 'N')
                    titleTexture.setTextureRect(IntRect(blockSize * 8, blockSize * 2, blockSize, blockSize));
                if (Field[i][j] == '1')
                    titleTexture.setTextureRect(IntRect(0, blockSize, blockSize, blockSize));
                if (Field[i][j] == '2')
                    titleTexture.setTextureRect(IntRect(blockSize, blockSize, blockSize, blockSize));
                if (Field[i][j] == '3')
                    titleTexture.setTextureRect(IntRect(blockSize * 2, blockSize, blockSize, blockSize));
                if (Field[i][j] == '4')
                    titleTexture.setTextureRect(IntRect(blockSize * 3, 0, blockSize, blockSize));
                if (Field[i][j] == '5')
                    titleTexture.setTextureRect(IntRect(blockSize * 4, blockSize * bullet[0].shot, blockSize, blockSize));
                if (Field[i][j] == '6')
                    titleTexture.setTextureRect(IntRect(blockSize * 5, blockSize * bullet[0].shot, blockSize, blockSize));
                if (Field[i][j] == '7')
                    titleTexture.setTextureRect(IntRect(blockSize * 6, blockSize * bullet[0].shot, blockSize, blockSize));
                if (Field[i][j] == '8')
                    titleTexture.setTextureRect(IntRect(blockSize * 7, blockSize * bullet[0].shot, blockSize, blockSize));
                if (Field[i][j] == 'W')
                    titleTexture.setTextureRect(IntRect(blockSize * 8, 0, blockSize, blockSize));
                if (Field[i][j] == ' ')
                    continue;

                titleTexture.setPosition(j * blockSize - offsetX, i * blockSize - offsetY);
                window.draw(titleTexture);
            }
        if (!win)
            window.draw(playerTexture.sprite);
        for (int i = 0; i < 6; i++)
            if (bullet[i].pusk)
                window.draw(bullet[i].sprite);
        for (int i = 0; i < 4; i++)
            window.draw(bat[i].sprite);
        for (int i = 0; i < 2; i++)
            window.draw(box[i].sprite);
        if (!game)
        {
            window.draw(youlose);
        }
        else if (win)
            window.draw(youwin);
        window.display();
    }
}

int main()
{
    texturesTreatment();
    return 0;
}