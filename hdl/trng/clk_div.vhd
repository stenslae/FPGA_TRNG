library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity clk_div is
    port (
        input  : in std_logic;
        div    : in std_logic_vector(6 downto 0);
        output : out std_logic
    );
end entity clk_div;

architecture clk_div of clk_div is

attribute keep : string;
attribute keep_hierarchy : string;

signal counter : std_logic_vector(6 downto 0) := (others => '0');
signal clk_out : std_logic := '0';
signal count_max : std_logic_vector(6 downto 0) := (others => '0');

attribute keep of clk_out           : signal is "true";
attribute keep of count_max         : signal is "true";
attribute keep of counter           : signal is "true";
attribute keep_hierarchy of clk_div : architecture is "true";

begin

	-- Create the pulse
	process (input)
	begin
		if rising_edge(input) then
            count_max <= div;
            if counter = count_max then
                counter <= (others => '0');
                clk_out <= '1';
            else
                clk_out <= '0';
                counter <= std_logic_vector(unsigned(counter) + 1);
            end if;
		end if;
	end process;

    output <= clk_out;

end architecture clk_div;