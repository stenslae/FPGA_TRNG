library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity trng is
    port (
    clk           : in std_logic;
    rst           : in std_logic;
    trng_register : out std_logic_vector(31 downto 0)
    );
end entity trng;

architecture trng of trng is

attribute keep           : string;
attribute keep_hierarchy : string;

signal gpio_sel   : std_logic_vector(1  downto 0) := "00";
signal gpio_h1    : std_logic := '0';
signal gpio_h2    : std_logic := '0';
signal gpio_sync1 : std_logic := '0';
signal gpio_sync2 : std_logic := '0';
signal en         : std_logic := '0';
signal ro_sample  : std_logic := '0';
signal vn_valid   : std_logic := '0';
signal entropy_1  : std_logic := '0';
signal entropy_2  : std_logic := '0';
signal div_clk    : std_logic := '0';

-- Random LFSR init seed
signal lfsr       : std_logic_vector(31 downto 0) := "10101011101010101010010111110100";
signal ro_sy2     : std_logic;
signal ro_sy1     : std_logic;
signal ro_mux     : std_logic;
signal ro1        : std_logic;
signal ro2        : std_logic;
signal ro3        : std_logic;
signal ro4        : std_logic;

attribute keep of ro1       : signal is "true";
attribute keep of ro2       : signal is "true";
attribute keep of ro3       : signal is "true";
attribute keep of ro4       : signal is "true";
attribute keep of ro_mux    : signal is "true";
attribute keep of gpio_sel  : signal is "true";
attribute keep of entropy_1 : signal is "true";
attribute keep of entropy_2 : signal is "true";
attribute keep of lfsr      : signal is "true";
attribute keep_hierarchy of trng : architecture is "true";

-- declare component
component muro is
    port (
    en     : in std_logic;
    div    : in std_logic_vector(6 downto 0);
    output : out std_logic
    );
end component muro;

component clk_div is
    port (
    input  : in std_logic;
    div    : in std_logic_vector(6 downto 0);
    output : out std_logic
    );
end component clk_div;

begin

    -- Build MURO
    ro1_dut : component muro
    port map (
        en     => en,
        div    => "1111111",
        output => ro1
    );
    ro2_dut : component muro
    port map (
        en     => en,
        div    => "1100001",
        output => ro2
    );
    ro3_dut : component muro
    port map (
        en     => en,
        div    => "1011100",
        output => ro3
    );
    ro4_dut : component muro
    port map (
        en     => en,
        div    => "1000000",
        output => ro4
    );

    cd1_dut : component clk_div
    port map (
        input  => clk,
        div    => "0010000",
        output => div_clk
    );
    
    -- Enable
    process(clk, rst)
    begin
        if rst = '1' then
            en        <= '0';
        elsif rising_edge(clk) then
            en <= '1';
        end if;
    end process;

    ro_mux <= ro1 xor ro2 xor ro3 xor ro4;

    -- D-FF Sample ro_mux
    process(div_clk, rst)
    begin
        if rst = '1' then
            ro_sample <= '0';
            ro_sy1    <= '0';
            ro_sy2    <= '0';
        elsif rising_edge(div_clk) then
            ro_sy1 <= ro_mux;
            ro_sy2 <= ro_sy1;

            ro_sample <= ro_sy2;
        end if;
    end process;

    -- Von-Neumann Correction
    process(div_clk, rst)
    begin
        if rst = '1' then
            vn_valid  <= '0';
            entropy_1 <= '0';
            entropy_2 <= '0';
        elsif rising_edge(div_clk) then
            -- VN correction
            if entropy_2 = '0' and ro_sample = '1' then
                vn_valid  <= '1';
                entropy_1 <= '1';
            elsif entropy_2 = '1' and ro_sample = '0' then
                vn_valid  <= '1';
                entropy_1 <= '0';
            else
                vn_valid  <= '0';
            end if;

            -- Save as previous
            entropy_2 <= ro_sample;
        end if;
    end process;

    -- 32‑bit LFSR whitening
    process(div_clk, rst)
    begin
        if rst = '1' then
            lfsr    <= "10101011101010101010010111110100";
        elsif rising_edge(div_clk) then
            if vn_valid = '1' then
                -- Using polynomial: x^32 + x^22 + x^17 + x^16 + 1
                lfsr <= lfsr(30 downto 0) & (lfsr(31) xor lfsr(21) xor lfsr(16) xor lfsr(15) xor entropy_1);
            end if;
        end if;
    end process;

    -- Save output into trng register
    trng_register <= lfsr;

end architecture trng;