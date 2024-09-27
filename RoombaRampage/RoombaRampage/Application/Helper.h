#pragma once

#include <memory>
/*--------------------------------------------------------------
  CURRENTLY NOT IN USE
--------------------------------------------------------------*/

namespace Helper {

	class Helpers {

	private:


	public:
		//singleton
		static Helpers* GetInstance() {
			if (!InstancePtr) {
				InstancePtr.reset(new Helpers{});
			}
			return InstancePtr.get();
		}

		float WindowWidth;

		float WindowHeight;

		float DeltaTime;

		float FpsCap;

		float Fps;

	private:
		static std::unique_ptr<Helpers> InstancePtr;

	};

}