library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity muro is
    port (
    en     : in std_logic;
    div    : in std_logic_vector(6 downto 0);
    output : out std_logic
    );
end entity muro;

architecture muro of muro is

attribute keep : string;
attribute keep_hierarchy : string;

signal ro1       : std_logic := '0';
signal ro2       : std_logic := '0';
signal ro3       : std_logic := '0';
signal ro4       : std_logic := '0';
signal ro5       : std_logic := '0';
signal ro6       : std_logic := '0';
signal ro7       : std_logic := '0';
signal ro8       : std_logic := '0';
signal ro8_div   : std_logic := '0';
signal ro_xor    : std_logic := '0';

attribute keep of ro1      : signal is "true";
attribute keep of ro2      : signal is "true";
attribute keep of ro3      : signal is "true";
attribute keep of ro4      : signal is "true";
attribute keep of ro5      : signal is "true";
attribute keep of ro6      : signal is "true";
attribute keep of ro7      : signal is "true";
attribute keep of ro8      : signal is "true";
attribute keep of ro8_div  : signal is "true";
attribute keep of ro_xor   : signal is "true";
attribute keep_hierarchy of muro : architecture is "true";

component ring_oscillator is
    port (
    clk    : in std_logic;
    en     : in std_logic;
    output : out std_logic
    );
end component ring_oscillator;

component ring_oscillator_async is
    port (
    en     : in std_logic;
    output : out std_logic
    );
end component ring_oscillator_async;

component clk_div is
    port (
    input  : in std_logic;
    div    : in std_logic_vector(6 downto 0);
    output : out std_logic
    );
end component clk_div;

begin

    ro1_dut : component ring_oscillator
    port map (
        clk    => ro8_div,
        en     => en,
        output => ro1
    );
    ro2_dut : component ring_oscillator
    port map (
        clk    => ro8_div,
        en     => en,
        output => ro2
    );
    ro3_dut : component ring_oscillator
    port map (
        clk    => ro8_div,
        en     => en,
        output => ro3
    );
    ro4_dut : component ring_oscillator
    port map (
        clk    => ro8_div,
        en     => en,
        output => ro4
    );
    ro5_dut : component ring_oscillator
    port map (
        clk    => ro8_div,
        en     => en,
        output => ro5
    );
    ro6_dut : component ring_oscillator
    port map (
        clk    => ro8_div,
        en     => en,
        output => ro6
    );
    ro7_dut : component ring_oscillator
    port map (
        clk    => ro8_div,
        en     => en,
        output => ro7
    );
    ro8_dut : component ring_oscillator_async
    port map (
        en     => en,
        output => ro8
    );
    cd_dut : component clk_div
    port map (
        input  => ro8,
        div    => div,
        output => ro8_div
    );

    ro_xor <= ro1 xor ro2 xor ro3 xor ro4 xor ro5 xor ro6 xor ro7;

    -- D-FF Samples the ro to prevent metastability
    process(ro8_div)
    begin
        if rising_edge(ro8_div) then
            output <= ro_xor;
        end if;
    end process;

end architecture muro;