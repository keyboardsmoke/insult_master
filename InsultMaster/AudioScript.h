#pragma once

enum class eAudioState
{
	HEY_MAN,
	YOU_DUMB,
	REBUKE_SOUND,
	BURNED,
	SAW_YOU_LOOKIN,
	JOE_MAMA,
	CLASSIC_COMEBACK,
	JOE_MAMA_X3_INCINERATION,
	INSULT_MASTER,
};

std::map<unsigned int, Track> sounds{
	{ static_cast<unsigned int>(eAudioState::HEY_MAN), Track(RESOURCE_FOLDER "sound/hey_man.mp3") },
	{ static_cast<unsigned int>(eAudioState::YOU_DUMB), Track(RESOURCE_FOLDER "sound/you_dumb.mp3") },
	{ static_cast<unsigned int>(eAudioState::REBUKE_SOUND), Track(RESOURCE_FOLDER "sound/rebuke.mp3") },
	{ static_cast<unsigned int>(eAudioState::BURNED), Track(RESOURCE_FOLDER "sound/burned.mp3") },
	{ static_cast<unsigned int>(eAudioState::SAW_YOU_LOOKIN), Track(RESOURCE_FOLDER "sound/saw_you.mp3") },
	{ static_cast<unsigned int>(eAudioState::JOE_MAMA), Track(RESOURCE_FOLDER "sound/joe_mama.mp3") },
	// rebuke sound plays here again
	{ static_cast<unsigned int>(eAudioState::CLASSIC_COMEBACK), Track(RESOURCE_FOLDER "sound/classic_comeback.mp3") },
	{ static_cast<unsigned int>(eAudioState::JOE_MAMA_X3_INCINERATION), Track(RESOURCE_FOLDER "/sound/joe_mama_incineration.mp3") },
	{ static_cast<unsigned int>(eAudioState::INSULT_MASTER), Track(RESOURCE_FOLDER "sound/u_win.mp3") }
};