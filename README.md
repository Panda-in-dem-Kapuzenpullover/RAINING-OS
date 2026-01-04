Most distributions force a choice between a bloated "friendly" UI or a hollow "minimal" base. RAINING-OS rejects that compromise. We ship a pre-hardened system with a custom C-compiled security plane, optimized for users who want invisible protection and a "stunning" desktop environment without the manual ricing labor.

Key Features
Security Stack 

RAINING-OS includes a multi-layered defense suite that monitors system integrity without user intervention.

Integrity Monitoring: rkhunter and chkrootkit configured via systemd timers.

Malware Analysis: maldet and ClamAV with optimized scan intervals.

Runtime Auditing: Falco for real-time kernel event monitoring.

Isolation: Firejail sandboxing for high-risk applications.

Access Control: Hardened AppArmor profiles active by default.

Performance

Liquorix Kernel: Zen-interactive tuned kernel for low-latency desktop use and gaming.

Minimal Overhead: Stripped-back background services; only what is essential for security and stability remains.

Ricing & UX

Department Lead: APRIL

KDE Plasma: Highly customized, performance-tuned environment.

Ready-to-Use: No "Stock Gnome" bloat. Modern layout, customized shaders, and low-latency input profiles are standard.

Architecture
Unlike script-heavy distros, RAINING-OS moves critical system settings into compiled C binaries for speed and security. Our firewall management tool, located in /src/security.c, interfaces directly with ufw to provide hardware-level lockdown presets.

Installation
Current build is available via the ISO releases tab.

Flash to USB using dd or BalenaEtcher.

Boot and follow the standard Calamares installer.

System security timers will initialize automatically on the first boot.

Roadmap
[ ] Web-based orchestration interface for remote updates.

[ ] SSH terminal bridge for centralized fleet management.

[ ] Transition to custom-built SELinux profiles.

Contributing
Later once we have more traction.

License
Copyright (C) 2026 Macaulay Smith.

Distributed under the GNU General Public License v3. See LICENSE for the full text.
