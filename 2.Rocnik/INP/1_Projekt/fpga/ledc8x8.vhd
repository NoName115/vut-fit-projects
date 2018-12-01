library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( -- Sem doplnte popis rozhrani obvodu.
	ROW : out std_logic_vector (0 to 7);
	LED : out std_logic_vector (0 to 7);
	RESET : in std_logic;
	SMCLK : in std_logic
);
end ledc8x8;

architecture main of ledc8x8 is

    -- Sem doplnte definice vnitrnich signalu.
	signal customCLK: std_logic;
	signal letterCHANGE: std_logic;
	signal letterREG: std_logic_vector (22 downto 0);
	signal customREG: std_logic_vector (7 downto 0);
	signal customROW: std_logic_vector (7 downto 0);

begin

    -- Sem doplnte popis funkce obvodu (zakladni konstrukce VHDL jako napr.
    -- prirazeni signalu, multiplexory, dekodery, procesy...).
    -- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL UVEDENE NA WEBU:
    -- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html

    -- Nezapomente take doplnit mapovani signalu rozhrani na piny FPGA
    -- v souboru ledc8x8.ucf.

	process(SMCLK, RESET, customCLK, customREG, letterREG, letterCHANGE)
	begin
	   if RESET = '1' then
	      customREG <= "00000000";
	      letterREG <= "00000000000000000000000";
	      letterCHANGE <= '0';
	   elsif SMCLK'event and SMCLK = '1' then
	      customREG <= customREG + 1;
	      letterREG <= letterREG + 1;
	   end if;

	   if letterREG(22) = '0'  then
		   letterCHANGE <= '1';
		else
			letterCHANGE <= '0';
		end if;

	   if customREG = "00000000" then
		   customCLK <= '1';
	   else
	   	customCLK <= '0';
	   end if;
	end process;


	process(RESET, customCLK, customROW)
	begin
	   if RESET = '1' then
	      customROW <= "10000000";
	   elsif customCLK'event and customCLK = '1' then
	      customROW <= customROW(0) & customROW(7 downto 1);
	   end if;
	end process;


	process(customROW, letterCHANGE)
	begin
	   if letterCHANGE = '1' then
	      case customROW is
	         when "10000000" => LED <= "10000011";
	         when "01000000" => LED <= "10111101";
	         when "00100000" => LED <= "10111101";
	         when "00010000" => LED <= "10000011";
	         when "00001000" => LED <= "10011111";
	         when "00000100" => LED <= "10101111";
	         when "00000010" => LED <= "10110111";
	         when "00000001" => LED <= "10111001";
	         when others     => LED <= "11111111";
		  end case;
	   else
	      case customROW is
	         when "10000000" => LED <= "10111100";
	         when "01000000" => LED <= "10111011";
	         when "00100000" => LED <= "10110111";
	         when "00010000" => LED <= "10101111";
	         when "00001000" => LED <= "10011111";
	         when "00000100" => LED <= "10101111";
	         when "00000010" => LED <= "10110111";
	         when "00000001" => LED <= "10111001";
	         when others     => LED <= "11111111";
		   end case;
	   end if;
   end process;

	ROW <= customROW;

end main;
