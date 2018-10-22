#include "pch.h"
#include "Memory.h"
#include "WitnessRandomizer.h"
#include <string>

int main(int argc, char** argv)
{
	if (argc == 2) {
		srand(atoi(argv[1])); // Seed with RNG from command line
	}
	WitnessRandomizer randomizer = WitnessRandomizer();

	std::vector<int> tutorialPanels = {
		// 0x0A3B5, // Back Left
		0x0A3B2, // Back Right
		0x00295, // Center Left
		0x00293, // Front Center
		0x002C2, // Front Left
		0x0C335, // Pillar
		0x0C373, // Patio floor
	};

	randomizer.Randomize(tutorialPanels, randomizer.RANDOMIZE_TARGETS);


	std::vector<int> panels = {
		0x3C12B, // Glass Factory Discard
		0x01A54, // Glass Factory Entry
		0x00086, // Glass Factory Vertical Symmetry 1
		0x00087, // Glass Factory Vertical Symmetry 2
		0x00059, // Glass Factory Vertical Symmetry 3
		0x00062, // Glass Factory Vertical Symmetry 4
		0x0005C, // Glass Factory Vertical Symmetry 5
		0x17CC8, // Glass Factory Summon Boat
		0x0008D, // Glass Factory Rotational Symmetry 1
		0x00081, // Glass Factory Rotational Symmetry 2
		0x00083, // Glass Factory Rotational Symmetry 3
		0x00084, // Glass Factory Melting 1
		0x00082, // Glass Factory Melting 2
		0x0343A, // Glass Factory Melting 3

		0x000B0, // Symmetry Island Door 1
		0x00022, // Symmetry Island Black Dots 1
		0x00023, // Symmetry Island Black Dots 2
		0x00024, // Symmetry Island Black Dots 3
		0x00025, // Symmetry Island Black Dots 4
		0x00026, // Symmetry Island Black Dots 5
		0x0007C, // Symmetry Island Colored Dots 1
		0x0007E, // Symmetry Island Colored Dots 2
		0x00075, // Symmetry Island Colored Dots 3
		0x00073, // Symmetry Island Colored Dots 4
		0x00077, // Symmetry Island Colored Dots 5
		0x00079, // Symmetry Island Colored Dots 6
		0x00065, // Symmetry Island Fading Lines 1
		0x0006D, // Symmetry Island Fading Lines 2
		0x00072, // Symmetry Island Fading Lines 3
		0x0006F, // Symmetry Island Fading Lines 4
		0x00070, // Symmetry Island Fading Lines 5
		0x00071, // Symmetry Island Fading Lines 6
		0x00076, // Symmetry Island Fading Lines 7
		// 0x009B8, // Symmetry Island Transparent 1 // Too mean for right now
		// 0x003E8, // Symmetry Island Transparent 2 // Too mean for right now
		// 0x00A15, // Symmetry Island Transparent 3 // Too mean for right now
		// 0x00B53, // Symmetry Island Transparent 4 // Too mean for right now
		// 0x00B8D, // Symmetry Island Transparent 5 // Too mean for right now
		// 0x1C349, // Symmetry Island Door 2 - Collision fails here, sadly
		0x00A52, // Symmetry Island Laser Yellow 1
		0x00A57, // Symmetry Island Laser Yellow 2
		0x00A5B, // Symmetry Island Laser Yellow 3
		0x00A61, // Symmetry Island Laser Blue 1
		0x00A64, // Symmetry Island Laser Blue 2
		0x00A68, // Symmetry Island Laser Blue 3
		// 0x0360D, // Symmetry Island Laser
	};
	randomizer.Randomize(panels, randomizer.RANDOMIZE_DATA);
}

void WitnessRandomizer::Randomize(std::vector<int> panels, int flags) {
	for (size_t i=panels.size() - 1; i > 1; i--) {
		int target = rand() % i;
		if (i != target) {
			if (flags & RANDOMIZE_TARGETS) {
				SwapPanelData(panels[i], panels[target], 0x2BC, 1);
			}
			if (flags & RANDOMIZE_DATA) {
				// TODO: Try using is_cylinder to swap into tutorial pillar. If it fails, discard.
				// Probably not: Extra back distance
				// TODO: This list should be way more explicit.
				SwapPanelData(panels[i], panels[target], 0x2FC, 125); // is_cylinder through max_connections
			}
			std::swap(panels[i], panels[target]); // Panel indices in the array
		}
	}
}

void WitnessRandomizer::SwapPanelData(int panel1, int panel2, int finalOffset, int dataSize) {
	// Currently wired for old version
	std::vector<int> panel1Offset = {0x5B28C0, 0x18, panel1*8, finalOffset};
	std::vector<int> panel2Offset = {0x5B28C0, 0x18, panel2*8, finalOffset};

	std::vector<int> panel1Data = _memory.ReadData<int>(panel1Offset, dataSize);
	std::vector<int> panel2Data = _memory.ReadData<int>(panel2Offset, dataSize);
	_memory.WriteData<int>(panel2Offset, panel1Data);
	_memory.WriteData<int>(panel1Offset, panel2Data);
}
