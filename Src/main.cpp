#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

// ---------------------------------------------------------------
// safenu: a mod menu containing only safe, non-cheating options.
// To add an option: add a setting in mod.json, then add a row in
// SafenuPopup::setup() with addToggle().
// ---------------------------------------------------------------

static void applySettings() {
    CCDirector::get()->setDisplayStats(Mod::get()->getSettingValue<bool>("show-fps"));
}

class SafenuPopup : public Popup<> {
protected:
    bool setup() override {
        this->setTitle("safenu");

        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        m_mainLayer->addChild(menu);

        auto size = m_mainLayer->getContentSize();
        float y = size.height - 60.f;

        addToggle(menu, "Show FPS Counter", "show-fps", {size.width / 2 - 70.f, y});

        auto note = CCLabelBMFont::create(
            "Only safe options. No cheats.", "chatFont.fnt"
        );
        note->setScale(0.7f);
        note->setOpacity(160);
        note->setPosition({size.width / 2, 22.f});
        m_mainLayer->addChild(note);

        return true;
    }

    void addToggle(CCMenu* menu, char const* text, char const* settingId, CCPoint pos) {
        auto toggler = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(SafenuPopup::onToggle), 0.7f
        );
        toggler->setPosition(pos);
        toggler->setUserObject("setting-id", CCString::create(settingId));
        toggler->toggle(Mod::get()->getSettingValue<bool>(settingId));
        menu->addChild(toggler);

        auto label = CCLabelBMFont::create(text, "bigFont.fnt");
        label->setScale(0.45f);
        label->setAnchorPoint({0.f, 0.5f});
        label->setPosition({pos.x + 22.f, pos.y});
        m_mainLayer->addChild(label);
    }

    void onToggle(CCObject* sender) {
        auto toggler = static_cast<CCMenuItemToggler*>(sender);
        auto id = static_cast<CCString*>(toggler->getUserObject("setting-id"));
        // The toggler flips its state after the callback, so invert.
        Mod::get()->setSettingValue<bool>(id->getCString(), !toggler->isToggled());
        applySettings();
    }

public:
    static SafenuPopup* create() {
        auto ret = new SafenuPopup();
        if (ret->initAnchored(260.f, 140.f)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

class $modify(SafenuMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        applySettings();

        auto btn = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSprite(
                "GJ_optionsBtn_001.png", 0.9f, CircleBaseColor::Green, CircleBaseSize::Medium
            ),
            this,
            menu_selector(SafenuMenuLayer::onSafenu)
        );
        btn->setID("safenu-button"_spr);

        if (auto menu = this->getChildByID("bottom-menu")) {
            menu->addChild(btn);
            menu->updateLayout();
        }
        return true;
    }

    void onSafenu(CCObject*) {
        SafenuPopup::create()->show();
    }
};

$on_mod(Loaded) {
    applySettings();
}
