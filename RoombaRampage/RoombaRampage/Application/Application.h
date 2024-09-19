#pragma once

namespace Application{

	class Application {

	public:

		static int Init();

		static int Run();

		static int Cleanup();

		
<<<<<<< HEAD
		
=======

	private:
		bool funcOnMouseMove(classMouseMoveEvent& givenEvent);
		static std::unique_ptr<AppWindow> lvWin;
		static Application* lvInstance;


>>>>>>> previous
	};



}