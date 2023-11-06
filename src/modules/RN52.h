#define RN52_H
#include <SafeString.h>
#include <Arduino.h>

struct AudioGateway {
  char connectedMAC[13];
  int batteryLevel = -1;
};

class RN52 {
	public:
		RN52();
		
		void begin();
		void setDiscoverable(bool discoverable);
		void toggleCommandEcho();

		void requestConfigUpdate();
		void requestAGBatteryLevel();
		void update();

		void increaseVolume();
		void decreaseVolume();
		void nextTrack();
		void previousTrack();
		void playPauseTrack();
		
		AudioGateway getAG();

		const char* getTrackMetaData();
	private:
		AudioGateway ag;
		void processBuffer();
};