#include "Game/SGameEngine.h"
// -- smart pointers delete themselves when there is no owner reference
// shared pointer = shares ownership across all references
// unique pointer = does not share ownership with anything else
// weak pointer = this has no ownership over any references

int main(int argc, char* argv[]) {
	int result = 0;

	// Initialise the engine
	// test if the int fails
	if (!SGameEngine::GetGameEngine()->Run()) {
		SGameEngine::DestroyGameEngine();
		result = -1;
	}

	// clean up the engine
	SGameEngine::DestroyGameEngine();

	return result;
}