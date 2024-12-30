#include "cocos.hpp"
#include <utils.hpp>

#include "nodes/ModalPopup.hpp"
#include "popup/options-popup.hpp"

namespace eclipse::gui::cocos {

void CocosRenderer::init() {
  // check if bitmap font exists
  if (!i18n::hasBitmapFont(i18n::getRequiredGlyphRangesString())) {
    geode::log::warn(
        "Bitmap font not found for selected language, setting to English");
    config::set("language", "en");
    i18n::setLanguage("en");
  }
}

void CocosRenderer::toggle() {
  if (m_popup)
    return shutdown();

  m_popup = Popup::create(Engine::get()->getTabs());
  m_popup->show();
  utils::updateCursorState(isToggled());
}

bool CocosRenderer::isToggled() const { return m_popup != nullptr; }

void CocosRenderer::shutdown() { shutdown(false); }

void CocosRenderer::shutdown(bool noCleanup) {
  for (auto popup : m_optionsPopups)
    popup->removeFromParentAndCleanup(true);
  m_optionsPopups.clear();

  for (auto modal : m_modals)
    modal->removeFromParentAndCleanup(true);
  m_modals.clear();

  if (!m_popup)
    return;

  if (!noCleanup)
    m_popup->removeFromParentAndCleanup(true);
  m_popup = nullptr;
  utils::updateCursorState(isToggled());
}

bool CocosRenderer::isPartOfPopup(cocos2d::CCNode *node) const {
  if (!m_popup)
    return false;
  return m_popup->isAncestorOf(node);
}

// since cocos is retained mode, extra work might not be needed
// might need to change in the future IDK
void CocosRenderer::queueAfterDrawing(const std::function<void()> &func) {
  func();
}

void CocosRenderer::showPopup(const eclipse::Popup &popup) {
  auto modal = ModalPopup::create(popup);
  modal->show();
  m_modals.push_back(modal);
}

void CocosRenderer::refreshPage() const {
  if (m_popup)
    m_popup->refreshPage();
}

std::string_view CocosRenderer::getSelectedTab() const {
  if (!m_popup)
    return "";
  auto tabs = Engine::get()->getTabs();
  auto idx = config::get<int>("menu.current_tab", 0);
  if (idx < 0 || idx >= tabs.size())
    return "";
  return tabs[idx]->getTitle();
}
} // namespace eclipse::gui::cocos
