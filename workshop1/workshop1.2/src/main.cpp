#include "utils.h"

// === Typedefs ===

using uint = unsigned int;

// === Constants ===

// Window
constexpr uint WINDOW_WIDTH = 800;
constexpr uint WINDOW_HEIGHT = 600;

const sf::Color WINDOW_BACKGROUND_COLOR = sf::Color(0xFF, 0xFF, 0xFF);

// Arrow
constexpr uint ARROW_INITIAL_X = WINDOW_WIDTH / 2;
constexpr uint ARROW_INITIAL_Y = WINDOW_HEIGHT / 2;
constexpr uint ARROW_ANGLE_OFFSET = 90;

constexpr float ARROW_BASE_WIDTH = 50.f;
constexpr float ARROW_BASE_HEIGHT = 50.f;

const sf::Color ARROW_FILL_COLOR = sf::Color(0xFF, 0xFF, 0);
const sf::Color ARROW_OUTLINE_COLOR = sf::Color(0, 0, 0);
const float ARROW_OUTLINE_THICKNESS = 2.f;

constexpr float ARROW_MAX_SPEED = 20.f;
constexpr float ARROW_MAX_ANGLE_SPEED = 90.f;

// === Data structures ===

struct Arrow
{
    sf::ConvexShape shape;
};

// === Function declarations ===

// Initialization
void initArrow(Arrow& arrow);

// Events handling
void pollEvents(sf::RenderWindow& window, sf::Vector2f& mousePosition);
void onMouseMove(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition);

// Rendering
void redrawFrame(sf::RenderWindow& window, Arrow& arrow);

// Updating
void update(Arrow& arrow, sf::Clock& clock, const sf::Vector2f& mousePosition);

// === Main program ===

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }),
        "Moving Arrow",
        sf::Style::Default,
        settings);

    sf::Clock clock;

    Arrow arrow;
    sf::Vector2f mousePosition;

    initArrow(arrow);
    while (window.isOpen())
    {
        pollEvents(window, mousePosition);
        update(arrow, clock, mousePosition);
        redrawFrame(window, arrow);
    }
}

// === Function definitions ===

void initArrow(Arrow& arrow)
{
    arrow.shape.setPointCount(7);
    arrow.shape.setPoint(0, { -ARROW_BASE_HEIGHT, ARROW_BASE_WIDTH / 2 });
    arrow.shape.setPoint(1, { -ARROW_BASE_HEIGHT, -(ARROW_BASE_WIDTH / 2) });
    arrow.shape.setPoint(2, { 0, -(ARROW_BASE_WIDTH / 2) });
    arrow.shape.setPoint(3, { 0, -ARROW_BASE_WIDTH });
    arrow.shape.setPoint(4, { ARROW_BASE_HEIGHT, 0 });
    arrow.shape.setPoint(5, { 0, ARROW_BASE_WIDTH });
    arrow.shape.setPoint(6, { 0, ARROW_BASE_WIDTH / 2 });

    arrow.shape.setFillColor(ARROW_FILL_COLOR);
    arrow.shape.setOutlineColor(ARROW_OUTLINE_COLOR);
    arrow.shape.setOutlineThickness(ARROW_OUTLINE_THICKNESS);

    arrow.shape.setPosition(ARROW_INITIAL_X, ARROW_INITIAL_Y);
}

void pollEvents(sf::RenderWindow& window, sf::Vector2f& mousePosition)
{
    sf::Event event{};
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseMoved:
            onMouseMove(event.mouseMove, mousePosition);
            break;
        default:
            break;
        }
    }
}

void redrawFrame(sf::RenderWindow& window, Arrow& arrow)
{
    window.clear(WINDOW_BACKGROUND_COLOR);
    window.draw(arrow.shape);
    window.display();
}

void onMouseMove(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition)
{
    std::cout << "mouse x=" << event.x << ", y=" << event.y << std::endl;

    mousePosition = { float(event.x), float(event.y) };
}

void update(Arrow& arrow, sf::Clock& clock, const sf::Vector2f& mousePosition)
{
    auto position = arrow.shape.getPosition();
    auto rotation = arrow.shape.getRotation();
    const auto dt = clock.restart().asSeconds();
    const auto delta = mousePosition - position;

    // Motion
    const auto direction = normVector(delta);
    position += ARROW_MAX_SPEED * direction * dt;

    // Rotation
    auto angle = toDegrees(std::atan2(delta.y, delta.x));
    if (angle < 0)
    {
        angle += 360;
    }

    const auto angleDiff = angle - rotation;
    if (std::abs(angleDiff) > 360 - std::abs(angleDiff))
    {
        if (angleDiff > 0)
        {
            rotation -= ARROW_MAX_ANGLE_SPEED * dt;
        }
        else
        {
            rotation += ARROW_MAX_ANGLE_SPEED * dt;
        }
    }
    else
    {
        if (angleDiff > 0)
        {
            rotation += ARROW_MAX_ANGLE_SPEED * dt;
        }
        else
        {
            rotation -= ARROW_MAX_ANGLE_SPEED * dt;
        }
    }

    arrow.shape.setPosition(position);
    arrow.shape.setRotation(rotation);
}
