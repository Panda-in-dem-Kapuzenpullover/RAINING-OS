# Step 1: Install necessary packages and extensions
sudo apt update && sudo apt upgrade -y
sudo apt install gnome-tweaks gnome-shell-extension-manager -y
sudo apt install gnome-shell-extensions gnome-shell-extension-dashtodock gnome-shell-extension-system-monitor -y
sudo apt install dconf-editor -y
sudo apt install gir1.2-gtop-2.0 gir1.2-nm-1.0 gir1.2-clutter-1.0 -y

# Step 2: Enable dark theme
gsettings set org.gnome.desktop.interface color-scheme 'prefer-dark'
gsettings set org.gnome.desktop.interface gtk-theme 'Adwaita-dark'
gsettings set org.gnome.desktop.interface icon-theme 'Adwaita'
gsettings set org.gnome.desktop.wm.preferences theme 'Adwaita-dark'

# Step 3: Enable extensions
gnome-extensions enable dash-to-dock@micxgx.gmail.com
gnome-extensions enable system-monitor@gnome-shell-extensions.gcampax.github.com

# Step 4: Configure Dash to Dock
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

# Step 5: Install Blur my Shell extension
cd /tmp
wget https://github.com/aunetx/blur-my-shell/archive/refs/heads/main.zip -O blur-my-shell.zip
unzip -o blur-my-shell.zip -d ~/.local/share/gnome-shell/extensions/blur-my-shell@aunetx
gnome-extensions enable blur-my-shell@aunetx

# Step 6: Configure Blur my Shell for panel transparency
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/blur true
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/override-background true
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/override-background-dynamically false
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/static-blur true
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/unblur-in-overview false
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/sigma 30
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/brightness 0.6
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/style-panel 0
dconf write /org/gnome/shell/extensions/blur-my-shell/panel/transparency 0.4

# Step 7: Configure Blur my Shell for dock
dconf write /org/gnome/shell/extensions/blur-my-shell/dash-to-dock/blur true
dconf write /org/gnome/shell/extensions/blur-my-shell/dash-to-dock/override-background true
dconf write /org/gnome/shell/extensions/blur-my-shell/dash-to-dock/static-blur true
dconf write /org/gnome/shell/extensions/blur-my-shell/dash-to-dock/style-dash-to-dock 0
dconf write /org/gnome/shell/extensions/blur-my-shell/dash-to-dock/sigma 30
dconf write /org/gnome/shell/extensions/blur-my-shell/dash-to-dock/brightness 0.6

# Step 8: Restart GNOME Shell (for Wayland)
busctl --user call org.gnome.Shell /org/gnome/Shell org.gnome.Shell Eval s 'Meta.restart("Restarting…")'
