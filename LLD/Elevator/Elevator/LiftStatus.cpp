#pragma once
enum class LiftStatus {
	MOVING_UP,
	MOVING_DOWN,
	MAINTAINACE,
	IDLE
};

inline string ToString(LiftStatus status) {
	switch (status) {
		case LiftStatus::IDLE: return "IDLE";
		case LiftStatus::MOVING_UP: return "MOVING_UP";
		case LiftStatus::MOVING_DOWN: return "MOVING_DOWN";
		case LiftStatus::MAINTAINACE: return "MAINTAINACE";
	}
}