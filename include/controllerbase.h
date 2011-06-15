#ifndef _CONTROLLER_BASE_H_
#define _CONTROLLER_BASE_H_

class ControllerBase {
public:
	ControllerBase() { };
	virtual ~ControllerBase() { };

	virtual bool left() const = 0;
	virtual bool right() const = 0;
	virtual bool down() const = 0;
	virtual bool rotateClockwise() const = 0;
	virtual bool rotateAntiClockwise() const = 0;
};

#endif
