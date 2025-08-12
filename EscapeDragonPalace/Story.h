#pragma once

bool GetStartStory();
void SetStartStory(bool src);
bool GetBossStory();
void SetBossStory(bool src);
bool GetEndStory();
void SetEndStory(bool src);

void CharacterDraw();
void PlayStartStory();
void PlayBossStory();
void PlayEndStory();