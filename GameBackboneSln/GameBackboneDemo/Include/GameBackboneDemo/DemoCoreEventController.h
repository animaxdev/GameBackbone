#pragma once

#include <GameBackboneDemo/MainMenuDemoRegion.h>

#include <GameBackbone/Core/CoreEventController.h>

#include <memory>

namespace EXE {
	class DemoCoreEventController : public GB::CoreEventController {
	public:
		// Constructors
		DemoCoreEventController();
		DemoCoreEventController(const DemoCoreEventController& other) = delete;
		DemoCoreEventController(DemoCoreEventController&& other) = default;
		DemoCoreEventController& operator=(const DemoCoreEventController& other) = delete;
		DemoCoreEventController& operator=(DemoCoreEventController&& other) = default;
		virtual ~DemoCoreEventController() = default;

	protected:
		// Event handling
		void handleEvent(sf::Event& event) override;
		void postHandleEvent();

	private:
		bool handleCoreEvent(sf::Event& event);
		bool handleGuiEvent(sf::Event& event);

		void postHandleEvent(sf::Event& event);
		void postDraw();

		sf::View m_camera;
		std::unique_ptr<MainMenuDemoRegion> m_mainMenuDemoRegion;
	};
}