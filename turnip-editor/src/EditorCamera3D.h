#pragma once
#include <TurnipEngine.h>

using namespace tur;

class EditorCamera3D : public PerspectiveCamera
{
public:
	EditorCamera3D() = default;

public:
	void SetWindowSize(int width, int height)
	{
		m_Width = width;
		m_Height = height;
	}

public:
	void OnEvent(IEvent& event)
	{
		Subscriber subscriber(event);
		subscriber.SubscribeTo<WindowResizeEvent>([&](WindowResizeEvent& window) -> bool {
			if (window.height == 0 || window.width == 0)
				return false;

			float aspectRatio = window.width / window.height;
			SetWindowSize(window.width, window.height);
			SetProjection(m_FOV, aspectRatio, m_Near, m_Far);
			
			return false;
		});

		HandleMovement();
	}

private:
	void HandleMovement()
	{
		glm::vec2 mousePosition = Mouse::position;
		glm::vec2 mouseDelta = glm::normalize(mousePosition - Mouse::positionLast);

		float scrollOffset = Mouse::scrollOffset.y;

		bool leftPressed = Mouse::Button(MouseButton::LEFT) == State::PRESS;
		bool rightPressed = Mouse::Button(MouseButton::RIGHT) == State::PRESS;
		bool middlePressed = Mouse::Button(MouseButton::MIDDLE) == State::PRESS;

		// Move:
		if (Keyboard::Key(Key::KEY_W) == State::PRESS)
			m_Position += m_Forward * m_Speed;

		if (Keyboard::Key(Key::KEY_S) == State::PRESS)
			m_Position += -m_Forward * m_Speed;

		if (Keyboard::Key(Key::KEY_A) == State::PRESS)
			m_Position += -glm::normalize(glm::cross(m_Forward, m_Up)) * m_Speed;

		if (Keyboard::Key(Key::KEY_D) == State::PRESS)
			m_Position += glm::normalize(glm::cross(m_Forward, m_Up)) * m_Speed;

		// Translate:
		if (middlePressed)
		{
			auto deltaMultipled = mouseDelta * m_Speed;
			m_Position += glm::vec3(-deltaMultipled.x, deltaMultipled.y, 0.f);
		}

		// Zoom:
		m_FOV -= scrollOffset;
		if (m_FOV < 1.f)
			m_FOV = 1.f;
		if (m_FOV > 90.f)
			m_FOV = 90.f;

		// Rotate:
		
		if (rightPressed)
		{
			float rotationX = (mousePosition.y - (m_Height / 2.f)) / m_Height;
			float rotationY = (mousePosition.x - (m_Height / 2.f)) / m_Height;

			glm::vec3 orientation = glm::rotate(m_Forward, glm::radians(rotationX), m_Right);
			if (!(glm::angle(orientation, m_Up) <= glm::radians(5.0f) || glm::angle(orientation, -m_Up) <= glm::radians(5.0f)))
			{
				m_Forward = orientation;
			}

			m_Forward = glm::rotate(m_Forward, glm::radians(-rotationY), m_Up);
		}
		

		SetProjection(FOV(), m_Width / m_Height, m_Near, m_Far);
	}

public:
	float FOV() const { return glm::radians(m_FOV); }

	float Near() const { return m_Near; }

	float Far() const { return m_Far; }

private:
	float m_FOV = 90.f;
	float m_Near = 0.1f;
	float m_Far = 100.f;
	const float m_Speed = 0.95f;

	int m_Width, m_Height;
};