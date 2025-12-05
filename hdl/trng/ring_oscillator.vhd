library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ring_oscillator is
    port (
    clk    : in std_logic;
    en     : in std_logic;
    output : out std_logic
    );
end entity ring_oscillator;

architecture ring_oscillator of ring_oscillator is

attribute keep : string;
attribute keep_hierarchy : string;

signal rot       : std_logic;
signal nad       : std_logic;
signal iv1       : std_logic;
signal iv2       : std_logic;
signal iv3       : std_logic;
signal iv4       : std_logic;
signal iv5       : std_logic;
signal iv6       : std_logic;

attribute keep of rot : signal is "true";
attribute keep of nad : signal is "true";
attribute keep of iv1 : signal is "true";
attribute keep of iv2 : signal is "true";
attribute keep of iv3 : signal is "true";
attribute keep of iv4 : signal is "true";
attribute keep of iv5 : signal is "true";
attribute keep of iv6 : signal is "true";
attribute keep_hierarchy of ring_oscillator : architecture is "true";

begin

    nad <= en and rot;
    iv1 <= not nad;
    iv2 <= not iv1;
    iv3 <= not iv2;
    iv4 <= not iv3;
    iv5 <= not iv4;
    iv6 <= not iv5;
    rot <= not iv6;

    -- D-FF Samples the ro to prevent metastability
    process(clk)
    begin
        if rising_edge(clk) then
            output <= rot;
        end if;
    end process;

end architecture ring_oscillator;