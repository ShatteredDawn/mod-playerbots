#pragma once

#include <cstdint>

enum class HunterAmmunitionsEnum : uint32_t
{
    // Arrows
    ROUGH_ARROW = 2512, // level 0
    SHARP_ARROW = 2515, // level 10
    RAZOR_ARROW = 3030, // level 25
    JAGGED_ARROW = 11285, // level 40
    ICE_THREADED_ARROW = 19316, // level 51 honored alterac valley
    WICKED_ARROW = 28053, // level 55 but pushed to 61 as it is TBC
    SCOUT_ARROW = 24417, // level 61 friendly cenarion expedition
    WARDEN_ARROW = 24412, // level 62 revered cenarion expedition
    BLACKFLIGHT_ARROW = 28056, // level 65
    HALAANI_RAZORSHAFT = 30611, // level 66 halaa
    TIMELESS_ARROW = 31737, // level 70 honored scale of the sands
    MYSTERIOUS_ARROW = 34581, // level 70 violet eye revered WOTLK
    TERRORSHAFT_ARROW = 41586, // level 75
    // Bullets
    LIGHT_SHOT = 2516, // level 0
    HEAVY_SHOT = 2519, // level 10
    SOLID_SHOT = 3033, // level 25
    ACCURATE_SLUGS = 11284, // level 40
    ICE_THREADED_BULLET = 19317, // level 51 honored alterac valley
    IMPACT_SHOT = 28060, // level 55 but pushed to 61 as it is TBC
    IRONBITE_SHELL = 28061, // level 65
    HALAANI_GRIMSHOT = 30612, // level 66 halaa
    HELLFIRE_SHOT = 32882, // level 68 revered thrallmar
    FELBANE_SLUGS = 32883, // level 68 revered honorhold
    TIMELESS_SHELL = 31735, // level 70 honored scale of the sands
    MYSTERIOUS_SHELL = 34582, // level 70 revered violet eye WOTLK
    FROSTBITE_BULLETS = 41584, // level 75
};
