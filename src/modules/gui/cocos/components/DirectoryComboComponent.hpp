#pragma once
#include <modules/gui/cocos/popup/options-popup.hpp>

#include "BaseComponentNode.hpp"

namespace eclipse::gui::cocos {
class FilesystemComboComponentNode
    : public BaseComponentNode<FilesystemComboComponentNode, cocos2d::CCMenu,
                               FilesystemComboComponent, float> {
 protected:
  TranslatedLabel *m_label = nullptr;
  CCMenuItemSpriteExtra *m_infoButton = nullptr;
  cocos2d::extension::CCScale9Sprite *m_background = nullptr;
  FallbackBMFont *m_valueLabel = nullptr;

  int m_index = 0;

 public:
  void updateLabel() const {
    if (m_index < 0 || m_index >= m_component->getItems().size()) {
      m_valueLabel->setString("");
      return;
    }
    m_valueLabel->setString(
        m_component->getItems()[m_index].filename().string());
    m_valueLabel->limitLabelWidth(200.f, 2.f, 0.25f);
  }

  void scroll(CCObject *sender) {
    int tag = sender->getTag();
    int value = m_index + tag;
    if (value < 0) value = std::max<int>(m_component->getItems().size() - 1, 0);
    if (value >= m_component->getItems().size()) value = 0;
    m_index = value;
    m_component->setValue(value);
    m_component->triggerCallback(value);
    updateLabel();
  }

  bool init(float width) override {
    if (!CCMenu::init()) return false;

    this->setID(fmt::format("filesystem-combo-{}"_spr, m_component->getId()));
    this->setContentSize({width, 28.f});

    auto labelSize = (width * 0.6f) - 35.f;

    if (!m_component->getDescription().empty()) {
      m_infoButton =
          geode::cocos::CCMenuItemExt::createSpriteExtraWithFrameName(
              "GJ_infoIcon_001.png", 0.5f,
              [this](auto) { this->openDescriptionPopup(); });
      m_infoButton->setAnchorPoint({0.5, 0.5f});
      this->addChildAtPosition(m_infoButton, geode::Anchor::Right,
                               {-10.f, 0.f});
      labelSize -= 15.f;
    }

    m_label = TranslatedLabel::create(m_component->getTitle());
    m_label->setAnchorPoint({0, 0.5f});
    m_label->limitLabelWidth(labelSize, 1.f, 0.25f);
    this->addChildAtPosition(m_label, geode::Anchor::Left, {15.f, 0.f});

    m_background =
        cocos2d::extension::CCScale9Sprite::create("square02b_001.png");
    m_background->setID("background");
    m_background->setAnchorPoint({0.5f, 0.5f});
    m_background->setScale(0.3f);
    m_background->setContentSize({365.f, 80.f});
    m_background->setColor(cocos2d::ccColor3B{50, 50, 50});
    m_background->setZOrder(-1);
    this->addChildAtPosition(m_background, geode::Anchor::Right, {-70.f, 0.f});

    auto spr =
        cocos2d::CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png");
    spr->setScale(0.6f);
    auto arrowBtn = CCMenuItemSpriteExtra::create(
        spr, this, menu_selector(FilesystemComboComponentNode::scroll));
    arrowBtn->setTag(1);
    this->addChildAtPosition(arrowBtn, geode::Anchor::Right, {-25.f, 0.f});

    auto arrowBtn2 = CCMenuItemSpriteExtra::create(
        spr, this, menu_selector(FilesystemComboComponentNode::scroll));
    arrowBtn2->setRotation(180.f);
    arrowBtn2->setTag(-1);
    this->addChildAtPosition(arrowBtn2, geode::Anchor::Right, {-115.f, 0.f});

    m_valueLabel =
        FallbackBMFont::create(m_component->getValue().filename().string());
    m_background->addChildAtPosition(m_valueLabel, geode::Anchor::Center,
                                     {0.f, 0.f});

    auto &items = m_component->getItems();
    if (items.empty()) {
      updateLabel();
      return true;
    }

    auto value = m_component->getValue();
    m_index = std::ranges::find(items, value) - items.begin();

    updateLabel();

    return true;
  }
};
}  // namespace eclipse::gui::cocos