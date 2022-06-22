#pragma once

struct StartSoundData
{
	float m_soundTiming;	// ms
	uint32_t m_cueID;
	bool m_played;

	void reset()
	{
		m_played = false;
	}
};

struct StartData
{
	std::string m_animationName;
	std::vector<StartSoundData> m_soundData;

	void reset()
	{
		for (StartSoundData& data : m_soundData)
		{
			data.reset();
		}
	}
};

class ColorsPhysics
{
public:
	static void applyPatches();

	static std::vector<StartData> m_startCrouchingData;
};

