#pragma once

#include "AnimBase.h"



class AnimDirectional : public AnimBase
{
protected:
	virtual void FrameStep() override;

	virtual void CropSprite() override;

	virtual void ReadIn(std::stringstream& stream) override;

private:

};

