/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
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

#include "symbolfontsprovider.h"

#include "global/stringutils.h"

#include "log.h"

using namespace mu;
using namespace mu::engraving;

void SymbolFontsProvider::addFont(const std::string& name, const std::string& family, const io::path_t& filePath)
{
    m_symbolFonts.push_back(std::make_shared<SymbolFont>(name, family, filePath));
    m_fallback.font = nullptr;
}

std::shared_ptr<SymbolFont> SymbolFontsProvider::doFontByName(const std::string& name) const
{
    std::string name_lo = mu::strings::toLower(name);
    for (const std::shared_ptr<SymbolFont>& f : m_symbolFonts) {
        if (mu::strings::toLower(f->name()) == name_lo) {
            return f;
        }
    }
    return nullptr;
}

ISymbolFontPtr SymbolFontsProvider::fontByName(const std::string& name) const
{
    std::shared_ptr<SymbolFont> font = doFontByName(name);
    if (!font) {
        font = doFallbackFont();
    }

    font->ensureLoad();
    return font;
}

std::vector<ISymbolFontPtr> SymbolFontsProvider::fonts() const
{
    std::vector<ISymbolFontPtr> fs;
    for (const std::shared_ptr<SymbolFont>& f : m_symbolFonts) {
        fs.push_back(f);
    }
    return fs;
}

void SymbolFontsProvider::setFallbackFont(const std::string& name)
{
    m_fallback.name = name;
    m_fallback.font = nullptr;
}

std::shared_ptr<SymbolFont> SymbolFontsProvider::doFallbackFont() const
{
    if (!m_fallback.font) {
        m_fallback.font = doFontByName(m_fallback.name);
        IF_ASSERT_FAILED(m_fallback.font) {
            return nullptr;
        }
    }

    return m_fallback.font;
}

ISymbolFontPtr SymbolFontsProvider::fallbackFont() const
{
    std::shared_ptr<SymbolFont> font = doFallbackFont();
    font->ensureLoad();
    return font;
}

bool SymbolFontsProvider::isFallbackFont(const ISymbolFont* f) const
{
    return doFallbackFont().get() == f;
}
