#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

static const size_t FILE_SZ           = 174;
static const size_t FILE_HASH         = 0x69936513130059A1;
static const size_t PATCHED_FILE_HASH = 0x7BD11D0C15004218;
static const size_t PATCH_POS         = 53;

int LoadScreen(void);
int ExitScreen(void);

int HackingProcessVizualization(void);
size_t djb2Hash(char *str, size_t str_len);

int main(int argc, char **argv)
{
    if(argc != 2) return EXIT_FAILURE;

    struct stat file_stat = {};
    int ex_stat = stat(argv[1], &file_stat);
    if(ex_stat == EOF) return EXIT_FAILURE;

    if(file_stat.st_size != FILE_SZ) return EXIT_FAILURE;
    char *buf = (char *)calloc(sizeof(char), FILE_SZ);

    FILE *patch_file = fopen(argv[1], "r+");
    fread(buf, sizeof(char), FILE_SZ, patch_file);
    fclose(patch_file);

    size_t hash = djb2Hash(buf, FILE_SZ);

    if(hash != FILE_HASH)
    {
        if(hash == PATCHED_FILE_HASH) printf("Already patched.\n");

        free(buf);
        return EXIT_FAILURE;
    }

    char jmp_code  = (char)0xEB;
    buf[PATCH_POS] = jmp_code;

    FILE *patched_file = fopen("PASSW_patched.COM", "wb");
    fwrite(buf, sizeof(char), FILE_SZ, patched_file);
    fclose(patched_file);

    free(buf);

    LoadScreen();
    ExitScreen();
}

int LoadScreen(void)
{
    sf::RenderWindow load_window(sf::VideoMode(800, 600), "load");

    float scale = 1.0f;

    sf::RectangleShape progress_bar(sf::Vector2f(600.f, 10.f));
    progress_bar.setFillColor(sf::Color::Black);
    progress_bar.setOutlineThickness(1.f);
    progress_bar.setOutlineColor(sf::Color::White);
    progress_bar.setPosition(sf::Vector2f(100.f, 500.f));

    sf::RectangleShape progress_bar_fill(sf::Vector2f(1.f, 10.f));
    progress_bar_fill.setPosition(sf::Vector2f(100.f, 500.f));

    sf::Texture load_background_texture;
    if(!load_background_texture.loadFromFile("img/man.jpg")) return EXIT_FAILURE;
    sf::Sprite load_background_sprite(load_background_texture);
    load_background_sprite.setScale(sf::Vector2f(0.7f, 0.5f));

    sf::Music music;
    if (!music.openFromFile("sounds/sisyphus.wav")) return EXIT_FAILURE;
    music.play();

    while(load_window.isOpen() && (progress_bar_fill.getScale().x < 600))
    {
        sf::Event event;
        while(load_window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                music.stop();
                load_window.close();
            }
        }

        load_window.clear(sf::Color::Black);
        progress_bar_fill.setScale(sf::Vector2f(scale, 1.f));
        scale += 0.01f;
        load_window.draw(load_background_sprite);
        load_window.draw(progress_bar);
        load_window.draw(progress_bar_fill);
        load_window.display();
    }
    music.stop();
    load_window.close();

    return EXIT_SUCCESS;
}

int ExitScreen(void)
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Взлом жопы");

    sf::Texture vzlom_background_texture;
    if(!vzlom_background_texture.loadFromFile("img/взлом_жопы.jpg")) return EXIT_FAILURE;

    sf::Sprite vzlom_background_sprite;
    vzlom_background_sprite.setTexture(vzlom_background_texture);
    vzlom_background_sprite.setScale(sf::Vector2f(1.5f, 2.1f));

    window.clear(sf::Color::Black);
    window.draw(vzlom_background_sprite);
    window.display();

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) window.close();
        }
    }
    return EXIT_SUCCESS;
}

size_t djb2Hash(char *str, size_t str_len)
{
    size_t hash = 5381;
    for(size_t i = 0; i < str_len; i++)
    {
        hash = ((hash << 5) + hash) + (size_t)str[i];
    }
    return hash;
}