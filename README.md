Mainline linux kernel for Orange Pi PC/Plus/One
-----------------------------------------------

This kernel tree is meant for Orange Pi PC, Orange Pi Plus
and Orange Pi One.

(You can easily port it to other similar H3 based SBCs by modifying the
appropriate board DTS files. Usually these boards are either like Orange Pi One,
or like Orange Pi PC, or they don't have CPUX voltage regulation at all, so it
should be simple.)

Features in addition to mainline:

- CPU frequency and voltage scaling (cpufreq)
- Thermal regulation (if CPU heats above certain temperature, it will try to cool itself down by reducing CPU frequency)
- Ethernet driver (v5 patches from [montjoie](https://github.com/montjoie/linux/commits/sun8i-emac-wip-v5))
- Working HDMI driver (v6 patches from [moinejf](http://moinejf.free.fr/opi2/) ported to sunxi-ng clk driver and fixed for 4.11)
- USB OTG (v4 patches from [Icenowy](https://groups.google.com/forum/m/#!topic/linux-sunxi/CgyTjm5BD-I))

Missing:

- Video decoding support

Have fun!

Kernel lockup issues
--------------------

*If you're getting lockups on boot or later during thermal regulation,
you're missing an u-boot patch.*

These lockups are caused by improper NKMP clock factors selection
in u-boot for PLL_CPUX. (M divider should not be used. P divider
should be used only for frequencies below 240MHz.)

This patch fixes it:

  0001-sunxi-h3-Fix-PLL1-setup-to-never-use-dividers.patch

Kernel side is already fixed in this kernel tree.

Sample configuration
--------------------

linux-4.11 file contains working configuration of the kernel
