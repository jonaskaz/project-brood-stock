#include <Dimmer.h>
#include <unity.h>

const int MAXBRIGHTNESS = 4095;
const int MINBRIGHTNESS = 500;
const long SUNRISELENGTH = 6000L;
const long SUNSETLENGTH = 6000L;

Dimmer dimmer(MAXBRIGHTNESS, MINBRIGHTNESS, SUNRISELENGTH, SUNSETLENGTH);

void testResetTiming() {
  dimmer.elapsedTime = 1000UL;
  dimmer.startBrightness = 10;
  dimmer.brightness = 0;
  dimmer.milliStart = 500000UL;
  dimmer.resetTiming();
  TEST_ASSERT(dimmer.startTime >= 500000UL);
  TEST_ASSERT(dimmer.elapsedTime == 0UL);
  TEST_ASSERT(dimmer.startBrightness == 0);
}

void testUpdateSunriseBrightness() {
  dimmer.startBrightness = dimmer.minBright;
  dimmer.brightness = 0UL;
  dimmer.elapsedTime = dimmer.sunriseLen;
  dimmer.updateSunriseBrightness();
  TEST_ASSERT(dimmer.brightness == dimmer.maxBright);
}

void testUpdateSunriseBrightnessMaxBrightLongElapsed() {
  dimmer.startBrightness = dimmer.maxBright;
  dimmer.brightness = dimmer.minBright;
  dimmer.elapsedTime = 500000UL;
  dimmer.updateSunriseBrightness();
  TEST_ASSERT(dimmer.brightness == dimmer.maxBright);
}

void testUpdateSunriseBrightnessLongElapsedTime() {
  dimmer.brightness = 0UL;
  dimmer.elapsedTime = 500000UL;
  dimmer.updateSunriseBrightness();
  TEST_ASSERT(dimmer.brightness == dimmer.maxBright);
}

void testUpdateSunsetBrightness() {
  dimmer.startBrightness = dimmer.maxBright;
  dimmer.brightness = dimmer.maxBright;
  dimmer.elapsedTime = dimmer.sunsetLen;
  dimmer.updateSunsetBrightness();
  TEST_ASSERT(dimmer.brightness == dimmer.minBright);
}

void testUpdateSunsetBrightnessMinBrightLongElapsed() {
  dimmer.startBrightness = dimmer.minBright;
  dimmer.brightness = dimmer.maxBright;
  dimmer.elapsedTime = 500000UL;
  dimmer.updateSunsetBrightness();
  TEST_ASSERT(dimmer.brightness == dimmer.minBright);
}

void testUpdateSunsetBrightnessLongElapsedTime() {
  dimmer.brightness = 0UL;
  dimmer.elapsedTime = 500000UL;
  dimmer.updateSunsetBrightness();
  TEST_ASSERT(dimmer.brightness == dimmer.minBright);
}

void testUpdateElapsedTime() {
  dimmer.startTime = 10000UL;
  dimmer.milliStart = 200000UL;
  dimmer.updateElapsedTime();
  TEST_ASSERT(dimmer.elapsedTime == 190000UL);
}

void testUpdateElapsedTimeOverflow() {
  dimmer.startTime = 4294967295UL;
  dimmer.milliStart = 5UL;
  dimmer.updateElapsedTime();
  TEST_ASSERT(dimmer.elapsedTime == 6UL);
}

void testMyMillis() {
  dimmer.milliStart = 500000UL;
  unsigned long timeNow = dimmer.myMillis();
  TEST_ASSERT(timeNow == 500000UL);
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(testMyMillis);
  RUN_TEST(testResetTiming);
  RUN_TEST(testUpdateElapsedTime);
  RUN_TEST(testUpdateElapsedTimeOverflow);
  RUN_TEST(testUpdateSunriseBrightness);
  RUN_TEST(testUpdateSunsetBrightness);
  RUN_TEST(testUpdateSunriseBrightnessLongElapsedTime);
  RUN_TEST(testUpdateSunsetBrightnessLongElapsedTime);
  RUN_TEST(testUpdateSunsetBrightnessMinBrightLongElapsed);
  RUN_TEST(testUpdateSunriseBrightnessMaxBrightLongElapsed);
  UNITY_END();
}
