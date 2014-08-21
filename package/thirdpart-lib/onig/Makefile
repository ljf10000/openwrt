include $(TOPDIR)/rules.mk

PKG_NAME:=onig
PKG_VERSION:=5.9.5
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://www.geocities.jp/kosako3/oniguruma/archive/

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_FIXUP:=autoreconf

include $(INCLUDE_DIR)/package.mk
####################################################################
define Package/onig
  CATEGORY:=thirdpart-lib
  TITLE:=onig
  DEPENDS:=
endef

define Build/Compile
	cp -fr files/* $(PKG_BUILD_DIR)/
	$(call Build/Compile/Default,)
endef

define Package/onig/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/.libs/libonig.so* $(1)/usr/lib/
endef
####################################################################
$(eval $(call BuildPackage,onig))

