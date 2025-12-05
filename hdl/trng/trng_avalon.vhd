library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity trng_avalon is
    port (
    clk           : in std_logic;
    rst           : in std_logic;
    -- avalon memory-mapped slave interface
    avs_read      : in std_logic;
    avs_write     : in std_logic;
    avs_address   : in std_logic_vector(1 downto 0);
    avs_readdata  : out std_logic_vector(31 downto 0);
    avs_writedata : in std_logic_vector(31 downto 0)
    );
end entity trng_avalon;

architecture trng_avalon of trng_avalon is

-- declare component
component trng is
    port (
    clk           : in std_logic;
    rst           : in std_logic;
    trng_register : out std_logic_vector(31 downto 0)
    );
end component trng;

-- internal signals to connect the registers to ports
signal trng_register   : std_logic_vector (31 downto 0) := (others => '0');

begin

    -- instantiate component
    trng_dut : component trng
    port map (
        clk            => clk,
        rst            => rst,
        trng_register  => trng_register
    );

    -- read
    avalon_register_read : process (clk)
    begin
        if rising_edge (clk) and avs_read = '1' then
            case avs_address is
                when "00"   => avs_readdata <= trng_register;
                when others => avs_readdata <= (others =>'0'); 
                -- return zeros for unused registers
            end case;
        end if;
    end process;

    -- write (no writing allowed to this register)

end architecture trng_avalon;