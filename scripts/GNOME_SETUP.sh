#!/bin/bash

# Step 1: Install necessary packages and extensions
sudo apt update && sudo apt upgrade -y
sudo apt install gnome-tweaks gnome-shell-extension-manager -y
sudo apt install gnome-shell-extensions gnome-shell-extension-dashtodock gnome-shell-extension-system-monitor -y
sudo apt install dconf-editor -y
sudo apt install gir1.2-gtop-2.0 gir1.2-nm-1.0 gir1.2-clutter-1.0 -y

# Step 2: Install build dependencies for Blur My Shell
sudo apt install git meson gettext -y

# Step 3: Enable dark theme
gsettings set org.gnome.desktop.interface color-scheme 'prefer-dark'
gsettings set org.gnome.desktop.interface gtk-theme 'Adwaita-dark'
gsettings set org.gnome.desktop.interface icon-theme 'Adwaita'
gsettings set org.gnome.desktop.wm.preferences theme 'Adwaita-dark'

# Step 4: Enable extensions
gnome-extensions enable dash-to-dock@micxgx.gmail.com
gnome-extensions enable system-monitor@gnome-shell-extensions.gcampax.github.com

# Step 5: Configure Dash to Dock
gsettings set org.gnome.shell.extensions.dash-to-dock dock-position 'LEFT'
gsettings set org.gnome.shell.extensions.dash-to-dock dash-max-icon-size 48
gsettings set org.gnome.shell.extensions.dash-to-dock extend-height false
gsettings set org.gnome.shell.extensions.dash-to-dock dock-fixed false
gsettings set org.gnome.shell.extensions.dash-to-dock autohide true
gsettings set org.gnome.shell.extensions.dash-to-dock intellihide true
gsettings set org.gnome.shell.extensions.dash-to-dock transparency-mode 'FIXED'
gsettings set org.gnome.shell.extensions.dash-to-dock background-opacity 0.1
gsettings set org.gnome.shell.extensions.dash-to-dock apply-custom-theme false
gsettings set org.gnome.shell.extensions.dash-to-dock customize-alphas true
gsettings set org.gnome.shell.extensions.dash-to-dock min-alpha 0.4
gsettings set org.gnome.shell.extensions.dash-to-dock max-alpha 0.8
gsettings set org.gnome.shell.extensions.dash-to-dock custom-theme-shrink true

# Step 6: Build and install Blur My Shell extension
cd /tmp
git clone https://github.com/aunetx/blur-my-shell.git
cd blur-my-shell
make

mkdir -p /usr/share/gnome-shell/extensions/blur-my-shell@aunetx
unzip build/blur-my-shell@aunetx.shell-extension.zip -d /usr/share/gnome-shell/extensions/blur-my-shell@aunetx/

glib-compile-schemas /usr/share/gnome-shell/extensions/blur-my-shell@aunetx/schemas/ 2>/dev/null || true

cd /tmp
rm -rf blur-my-shell

# Step 7: Enable Blur My Shell
gnome-extensions enable blur-my-shell@aunetx

# Step 8: Configure Blur My Shell for panel
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/blur true
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/override-background true
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/override-background-dynamically false
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/static-blur true
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/unblur-in-overview false
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/sigma 30
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/brightness 0.6
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/style-panel 0
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/transparency 0.4

# Step 9: Configure Blur My Shell for dock
dconf write /org/gnome/shell/extensions/blur-my-shell/dash-to-dock/blur true
dconf write /org/gnome/shell/extensions/blur-my-shell/dash-to-dock/override-background true
dconf write /org/gnome/shell/extensions/blur-my-shell/dash-to-dock/static-blur true
dconf write /org/gnome/shell/extensions/blur-my-shell/dash-to-dock/style-dash-to-dock 0
dconf write /org/gnome/shell/extensions/blur-my-shell/dash-to-dock/sigma 30
dconf write /org/gnome/shell/extensions/blur-my-shell/dash-to-dock/brightness 0.6
