// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/StateBase.h"

bool UStateBase::IsTickable() const
{
	// オブジェクト単体でTickはさせない→マシンから呼ぶ想定
	return false;
}
