/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2024 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "soundflag.h"

#include "undo.h"

using namespace mu::engraving;

SoundFlag::SoundFlag(EngravingItem* parent)
    : EngravingItem(ElementType::SOUND_FLAG, parent)
{
    m_iconFont = draw::Font(engravingConfiguration()->iconsFontFamily(), draw::Font::Type::Icon);

    static constexpr double DEFAULT_FONT_SIZE = 8.0;
    m_iconFont.setPointSizeF(DEFAULT_FONT_SIZE);
}

SoundFlag* SoundFlag::clone() const
{
    return new SoundFlag(*this);
}

bool SoundFlag::isEditable() const
{
    return false;
}

void SoundFlag::setSelected(bool f)
{
    EngravingItem* parent = parentItem();
    if (parent) {
        parent->setSelected(f);
    }

    EngravingItem::setSelected(f);
}

const SoundFlag::PresetCodes& SoundFlag::soundPresets() const
{
    return m_soundPresets;
}

void SoundFlag::setSoundPresets(const PresetCodes& soundPresets)
{
    m_soundPresets = soundPresets;
}

const SoundFlag::PlayingTechniqueCodes& SoundFlag::playingTechniques() const
{
    return m_playingTechniques;
}

void SoundFlag::setPlayingTechniques(const PlayingTechniqueCodes& techniques)
{
    m_playingTechniques = techniques;
}

bool SoundFlag::shouldHide() const
{
    if (const Score* score = this->score()) {
        if (!score->showSoundFlags()) {
            return true;
        }

        if (score->printing()) {
            return true;
        }
    }

    if (!m_soundPresets.empty() || !m_playingTechniques.empty()) {
        return false;
    }

    if (selected()) {
        return false;
    }

    const EngravingItem* parent = parentItem();
    if (parent && parent->selected()) {
        return false;
    }

    return true;
}

void SoundFlag::undoChangeSoundFlag(const PresetCodes& presets, const PlayingTechniqueCodes& techniques)
{
    if (m_soundPresets == presets && m_playingTechniques == techniques) {
        return;
    }

    score()->undo(new ChangeSoundFlag(this, presets, techniques));
}

char16_t SoundFlag::iconCode() const
{
    return 0xEF4E;
}

draw::Font SoundFlag::iconFont() const
{
    return m_iconFont;
}

void SoundFlag::setIconFontSize(double size)
{
    m_iconFont.setPointSizeF(size);
}

Color SoundFlag::iconBackgroundColor() const
{
    Color color = curColor();
    if (!selected()) {
        color = Color("#CFD5DD");
        color.setAlpha(128);
    }

    return color;
}
