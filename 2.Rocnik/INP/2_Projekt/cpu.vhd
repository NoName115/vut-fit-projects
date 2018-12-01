-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2016 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): xkolcu00, xkolcu00@stud.fit.vutbr.cz
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	-- zde dopiste potrebne deklarace signalu
	type my_states is ( state_start, state_fetch0, state_fetch1, state_inc_pointer, state_dec_pointer, state_inc_value0, state_inc_value1,
							  state_inc_value2, state_dec_value0, state_dec_value1, state_dec_value2, state_putchar0, state_putchar1, state_putchar2,
							  state_getchar0, state_getchar1, state_getchar2, state_store_help_reg,
							  state_store_to_tmp0, state_store_to_tmp1, state_store_to_tmp2, state_load_from_tmp,
							  state_while_start0, state_while_start1, state_while_start2, state_while_start3, state_while_start4, state_while_start5, state_while_start6, state_while_start7,
							  state_while_end0, state_while_end1, state_while_end2, state_while_end3, state_while_end4, state_while_end5, state_while_end6, state_while_end7, state_while_end8,
							  state_decode, state_comment, state_end );

	signal current_state : my_states;
	signal next_state : my_states;
 
	-- PC - programovy citac
	-- signaly pre PC register
	signal pc_inc : std_logic;
	signal pc_dec : std_logic;
	signal pc_reg : std_logic_vector(11 downto 0); -- Podla velkosti adresy v ROM
	signal pc_set : std_logic; -- Urcuje si sa pc_reg da na CODE_ADDR

	-- INSTRUCTIONS - premenne pre instrukcie
	signal ins_reg : std_logic_vector(7 downto 0); -- Podla velkosti dat v ROM, uklada sa nedekodovana instrukcia
	type instruction_type is ( inst_inc_pointer, inst_dec_pointer, inst_inc_value, inst_dec_value, inst_while_start, inst_while_end,
										inst_putchar, inst_getchar, inst_store_to_tmp, inst_load_from_tmp, inst_end, inst_comment );
	signal ins_reg_decoded : instruction_type; -- Dekodovana instrukcia
	signal ins_set : std_logic; -- Urcuje ci sa do ins_reg da CODE_DATA
	
	-- PTR - register
	signal ptr_inc : std_logic;
	signal ptr_dec : std_logic;
	signal ptr_set : std_logic; -- Urcuje ci sa ptr_reg da na DATA_ADDR
	signal ptr_reg : std_logic_vector(9 downto 0); -- Podla velkosti adresy v RAM
	
	-- TMP - register
	signal tmp_reg : std_logic_vector(7 downto 0);	-- Podla velkosti dat v RAM
	signal tmp_reg_write_data : std_logic;				-- zapise tmp_reg do DATA_WDATA

	-- Pomocny register
	signal help_reg_inc : std_logic;
	signal help_reg_dec : std_logic;
	signal help_reg : std_logic_vector(7 downto 0);
	signal help_reg_load_from_ram : std_logic;
	signal help_reg_store_input : std_logic;		-- uklada IN_DATA do help_reg
	signal help_reg_write_data : std_logic;		-- zapise data do DATA_WDATA
	signal help_reg_write_outdata : std_logic;	-- zapise help_reg do OUT_DATA
	
	-- CNT - register pre cyklus while
	signal cnt_inc : std_logic;		-- cnt_reg--
	signal cnt_dec : std_logic;		-- cnt_reg++
	signal cnt_init : std_logic;		-- inicializuje cnt_reg na hodnotu 1
	signal cnt_reg : std_logic_vector(7 downto 0); 	-- pocita zatvorky

begin

	-- zde dopiste vlastni VHDL kod

	-- Hlavny proces pre PC
	process (RESET, CLK)
	begin
		-- vynulovanie pri RESET
		if (RESET = '1') then
			pc_reg <= (others => '0');
			
		-- pri nastupnej hrane
		elsif (CLK'event) and (CLK = '1') then
			if (pc_inc = '1') then
				pc_reg <= pc_reg + 1;
			elsif (pc_dec = '1') then
				pc_reg <= pc_reg - 1;
			end if;
		end if;
	end process;
	
	-- trojstavovy budic
	-- prenesenie hodnoty z pc_reg do CODE_ADDR
	-- 'Z' stav vysokej impedancie
	-- pc_reg je odpojeny od CODE_ADDR
	CODE_ADDR <= pc_reg when (pc_set = '1')
			else (others => 'Z');


	-- Hlavny proces pre INSTRUCTION
	process (RESET, CLK)
	begin
		-- vynulovanie pri RESET
		if (RESET = '1') then
			ins_reg <= (others => '0');
			
		elsif (CLK'event) and (CLK = '1') then
			if (ins_set = '1') then
				ins_reg <= CODE_DATA;	-- Nacita nedekodovanu instrukciu
			end if;
		end if;
	end process;
	
	-- Dekodovanie instrukcie
	process (ins_reg)
	begin
		case (ins_reg(7 downto 4)) is
		
			when X"0" =>
				case (ins_reg(3 downto 0)) is
					when X"0" =>
						ins_reg_decoded <= inst_end;		--	NULL
					when others =>
						ins_reg_decoded <= inst_comment;	--	comment
				end case;
				
			when X"2" =>
				case (ins_reg(3 downto 0)) is
					when X"1" =>
						ins_reg_decoded <= inst_load_from_tmp;	-- '!'
					when X"4" =>
						ins_reg_decoded <= inst_store_to_tmp;	-- '$'
					when X"C" =>
						ins_reg_decoded <= inst_getchar;			-- ','
					when X"E" =>
						ins_reg_decoded <= inst_putchar;			-- '.'
					when X"D" =>
						ins_reg_decoded <= inst_dec_value;		-- '-'
					when X"B" =>
						ins_reg_decoded <= inst_inc_value;		-- '+'
					when others =>
						ins_reg_decoded <= inst_comment;			-- comment
				end case;
			
			when X"3" =>
				case (ins_reg(3 downto 0)) is
					when X"C" =>
						ins_reg_decoded <= inst_dec_pointer;	-- '<'
					when X"E" =>
						ins_reg_decoded <= inst_inc_pointer;	-- '>'
					when others =>
						ins_reg_decoded <= inst_comment;			--	comment
				end case;
				
			when X"5" =>
				case (ins_reg(3 downto 0)) is
					when X"D" =>
						ins_reg_decoded <= inst_while_end; 		-- ']'
					when X"B" =>
						ins_reg_decoded <= inst_while_start;	-- '['
					when others =>
						ins_reg_decoded <= inst_comment;			-- comment
				end case;
				
			when others	=>
				ins_reg_decoded <= inst_comment;		-- comment
		end case;
	end process;


	-- proces pre PTR register
	process (RESET, CLK)
	begin
		if (RESET = '1') then
			ptr_reg <= (others => '0');
			
		elsif (CLK'event) and (CLK = '1') then
			if (ptr_inc = '1') then
				ptr_reg <= ptr_reg + 1;
			elsif (ptr_dec = '1') then
				ptr_reg <= ptr_reg - 1;
			end if;
		end if;
	end process;
	
	
	-- trojstavovy budic pre ptr_reg a DATA_ADDR
	DATA_ADDR <= ptr_reg when (ptr_set = '1')
			else (others => 'Z');

	
	----------------------------------------------
	-- proces pre CNT register
	process (RESET, CLK)
	begin
		if (RESET = '1') then
			cnt_reg <= (others => '0');
			
		elsif (CLK'event) and (CLK = '1') then
			if (cnt_init = '1') then
				cnt_reg(7 downto 0) <= "00000001";
			elsif (cnt_inc = '1') then
				cnt_reg <= cnt_reg + 1;
			elsif (cnt_dec = '1') then
				cnt_reg <= cnt_reg - 1;
			end if;
		end if;
	end process;
	
	
	----------------------------------------------
	-- proces pre HELP register
	process (RESET, CLK)
	begin
		if (RESET = '1') then
			help_reg <= (others => '0');
			
		elsif (CLK'event) and (CLK = '1') then
			if (help_reg_load_from_ram = '1') then
				help_reg <= DATA_RDATA;
			elsif (help_reg_store_input = '1') then
				help_reg <= IN_DATA;
			elsif (help_reg_inc = '1') then
				help_reg <= help_reg + 1;
			elsif (help_reg_dec = '1') then
				help_reg <= help_reg - 1;
			end if;
		end if;
	end process;
	
	DATA_WDATA <=  help_reg when (help_reg_write_data = '1') else
						tmp_reg	when (tmp_reg_write_data = '1')
							else (others => 'Z');
			
	OUT_DATA <= help_reg when (help_reg_write_outdata = '1')
			else (others => 'Z');
	
	----------------------------------------------
	-- proces pre zmenu stavu current_state
	process (RESET, CLK)
	begin
		if (RESET = '1') then
			current_state <= state_start;
			
		elsif (CLK'event) and (CLK = '1') then
			if (EN = '1') then
				current_state <= next_state;
			end if;
		end if;
	end process;
	
	
	-- proces stavoveho automatu
	process (current_state, ins_reg_decoded, IN_VLD, OUT_BUSY, help_reg, cnt_reg)
	begin
		-- Nulovanie signalov
		next_state <= state_start;
		
		pc_inc <= '0';
		pc_dec <= '0';
		pc_set <= '0';
		
		CODE_EN <= '0';
		DATA_EN <= '0';
		DATA_RDWR <= '0';
		
		OUT_WE <= '0';
		IN_REQ <= '0';
		
		ptr_inc <= '0';
		ptr_dec <= '0';
		ptr_set <= '0';
		
		cnt_inc <= '0';
		cnt_dec <= '0';
		cnt_init <= '0';
		
		ins_set <= '0';
		
		help_reg_inc <= '0';
		help_reg_dec <= '0';
		help_reg_load_from_ram <= '0';
		help_reg_store_input <= '0';
		help_reg_write_data <= '0';
		help_reg_write_outdata <= '0';
		
		
		case(current_state) is
			
			when state_start =>
				next_state <= state_fetch0;
				
			-- Nacitanie instrukcie
			when state_fetch0 =>
				next_state <= state_fetch1;
				pc_set <= '1';		-- do CODE_ADDR <= pc_reg
				CODE_EN <= '1';	-- aktualizuje CODE_DATA
				
			when state_fetch1 =>
				next_state <= state_decode;
				ins_set <= '1';	-- CODE_DATA <= ins_reg a nastavi ins_reg_decoded
				
			-- Prechod do dalsich statov vhladom na vstupnu instrukciu
			-- 	inst_inc_pointer, inst_dec_pointer, inst_inc_value, inst_dec_value, inst_while_start, inst_while_end,
			--		inst_putchar, inst_getchar, inst_store_to_tmp, inst_load_from_tmp, inst_end, inst_comment
			when state_decode =>
				case ins_reg_decoded is
					when inst_inc_pointer =>
						next_state <= state_inc_pointer;
					when inst_dec_pointer =>
						next_state <= state_dec_pointer;
					when inst_inc_value =>
						next_state <= state_inc_value0;
					when inst_dec_value =>
						next_state <= state_dec_value0;
					when inst_while_start =>
						next_state <= state_while_start0;
					when inst_while_end =>
						next_state <= state_while_end0;
					when inst_putchar =>
						next_state <= state_putchar0;
					when inst_getchar =>
						next_state <= state_getchar0;
					when inst_store_to_tmp =>
						next_state <= state_store_to_tmp0;
					when inst_load_from_tmp =>
						next_state <= state_load_from_tmp;
					when inst_end =>
						next_state <= state_end;
					when inst_comment =>
						next_state <= state_comment;
					
					when others =>
						next_state <= state_end;
				end case;
			
			---------------------------------------
			---------------------------------------
			-- ptr_reg++
			when state_inc_pointer =>
				next_state <= state_fetch0;
				pc_inc <= '1';
				ptr_inc <= '1';
			
			---------------------------------------
			-- ptr_reg--
			when state_dec_pointer =>
				next_state <= state_fetch0;
				pc_inc <= '1';
				ptr_dec <= '1';
				
			
			---------------------------------------
			---------------------------------------
			-- *ptr_reg++
			when state_inc_value0 =>
				next_state <= state_inc_value1;
				ptr_set <= '1';		-- na DATA_ADDR nastavy ptr_reg
				DATA_EN <= '1';		-- povoli pracu s RAM
				DATA_RDWR <= '1';		-- aktualizuje DATA_RDATA
				
			when state_inc_value1 =>
				next_state <= state_inc_value2;
				help_reg_load_from_ram <= '1';	-- nacita DATA_RDATA do help_reg
				
			when state_inc_value2 =>
				next_state <= state_store_help_reg;
				help_reg_inc <= '1';		-- help_reg++
				
			---------------------------------------
			-- *ptr_reg--
			when state_dec_value0 =>
				next_state <= state_dec_value1;
				ptr_set <= '1';
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				
			when state_dec_value1 =>
				next_state <= state_dec_value2;
				help_reg_load_from_ram <= '1';	-- nacita DATA_RDATA do help_reg
				
			when state_dec_value2 =>
				next_state <= state_store_help_reg;
				help_reg_dec <= '1';		-- help_reg
			
			---------------------------------------
			-- ulozi help_reg na adresu DATA_ADDR
			when state_store_help_reg =>
				next_state <= state_fetch0;
				ptr_set <= '1';					-- nastavi DATA_ADDR
				help_reg_write_data <= '1';	-- do DATA_WDATA da help_reg
				DATA_EN <= '1';		-- povoli pracu s RAM
				DATA_RDWR <= '0';		-- aktualizuje data na DATA_ADDR a DATA_RDATA
				pc_inc <= '1';			-- ide na dalsiu instrukciu
				
				
			---------------------------------------
			---------------------------------------
			-- zaciatok WHILE
			when state_while_start0 =>
				next_state <= state_while_start1;
				ptr_set <= '1';		-- nastavim DATA_ADDR
				DATA_EN <= '1';		-- povolim pracu s RAM
				DATA_RDWR <= '1';		-- aktualizujem DATA_RDATA
			
			when state_while_start1 =>
				next_state <= state_while_start2;
				help_reg_load_from_ram <= '1';	-- dam DATA_RDATA do help_reg
				
			when state_while_start2 =>
				pc_inc <= '1';					-- posunie hlavu pre ROM
				if (help_reg(7 downto 0) = "00000000") then
					cnt_init <= '1';			-- inicalizuje cnt_reg na 1
					next_state <= state_while_start3;
				else
					next_state <= state_fetch0;
				end if;
			
			-- caka na incializaciu cnt_reg
			when state_while_start3 =>
				next_state <= state_while_start4;
				
			-- ignorovat vsetko az po pravu zatvorku
			-- robi v podstate fetch0..n
			when state_while_start4 =>
				-- som vo vnutry cyklu a musim ignorovat
				if (cnt_reg /= "00000000") then
					pc_set <= '1';
					CODE_EN <= '1';
					next_state <= state_while_start5;
				else
					-- ingnoroval som vsetko, pokracujem v programe
					next_state <= state_fetch0;
				end if;
				
			when state_while_start5 =>
				next_state <= state_while_start6;
				ins_set <= '1';		-- dekoduje instrukciu
				
			when state_while_start6 =>
				next_state <= state_while_start7;
				pc_inc <= '1';			-- posunie hlavu ROM
				
				case (ins_reg_decoded) is
					when inst_while_start =>
						cnt_inc <= '1';
					when inst_while_end =>
						cnt_dec <= '1';
					when others =>
						null;
				end case;
			
			-- cakanie na inc/dec cnt_reg
			when state_while_start7 =>
				next_state <= state_while_start4;
			
			
			---------------------------------------
			-- koniec WHILE
			when state_while_end0 =>
				next_state <= state_while_end1;
				ptr_set <= '1';
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				
			when state_while_end1 =>
				next_state <= state_while_end2;
				help_reg_load_from_ram <= '1';
				
			when state_while_end2 =>
				if (help_reg(7 downto 0) = "00000000") then
					pc_inc <= '1';
					next_state <= state_fetch0;
				else
					cnt_init <= '1';		-- inicializuje cnt_reg na 1
					pc_dec <= '1';			-- posunie hlavu pre ROM
					next_state <= state_while_end3;
				end if;
			
			-- caka na inicializaciu cnt_reg
			when state_while_end3 =>
				next_state <= state_while_end4;
			
			-- ignoruje vsetko az po lavu zatvorku
			-- robi do lava fetch a hlada lavu zatvorku
			when state_while_end4 =>
				if (cnt_reg /= "00000000") then
					next_state <= state_while_end5;
					pc_set <= '1';
					CODE_EN <= '1';
				else
					next_state <= state_fetch0;
				end if;
				
			when state_while_end5 =>
				next_state <= state_while_end6;
				ins_set <= '1';	-- dekoduje dalsiu instrukciu
			
			when state_while_end6 =>
				next_state <= state_while_end7;
				
				case (ins_reg_decoded) is
					when inst_while_start =>
						cnt_dec <= '1';
					when inst_while_end =>
						cnt_inc <= '1';
					when others =>
						null;
				end case;
				
			when state_while_end7 =>
				next_state <= state_while_end8;
			
			-- Kvoli poslednej [ zatvorke kde dam cnt_dec <= '1'
			-- ale cnt_reg je stale 1 nie 0, musim pockat jeden takt
			-- aby bol cnt_reg 0
			when state_while_end8 =>
				next_state <= state_while_end4;
				
				if (cnt_reg /= "00000000") then
					pc_dec <= '1';
				else
					pc_inc <= '1';
				end if;


			---------------------------------------
			---------------------------------------
			-- pise znak na obrazovku putchar(*ptr)
			when state_putchar0 =>
				next_state <= state_putchar1;
				ptr_set <= '1';		-- nastavai DATA_ADDR
				DATA_EN <= '1';		-- povoli pracu s RAM
				DATA_RDWR <= '1';		-- aktualizuje DATA_RDATA
				
			when state_putchar1 =>
				next_state <= state_putchar2;
				help_reg_load_from_ram <= '1'; -- nacita DATA_RDATA do help_reg
				
			when state_putchar2 =>
				if (OUT_BUSY = '0') then
					OUT_WE <= '1';
					help_reg_write_outdata <= '1';	-- do OUT_DATA da help_reg
					pc_inc <= '1';						-- posunie ukazovatel do ROM
					next_state <= state_fetch0;	-- cita dalsiu instrukciu
				else
					next_state <= state_putchar2;		-- caka kym OUT_BUSY je 0
				end if;
			
			
			---------------------------------------
			---------------------------------------
			-- berie znak zo vstupu *ptr = getchar()
			when state_getchar0 =>
				next_state <= state_getchar1;
				IN_REQ <= '1';
				
			when state_getchar1 =>
				if (IN_VLD = '1') then
					next_state <= state_getchar2;
					help_reg_store_input <= '1';
				else
					next_state <= state_getchar1;
				end if;
				
			when state_getchar2 =>
				next_state <= state_store_help_reg;
			
			
			---------------------------------------
			---------------------------------------
			-- Ulozi hodnotu ptr do tmp, tmp = *ptr;
			when state_store_to_tmp0 =>
				next_state <= state_store_to_tmp1;
				ptr_set <= '1';		-- nastavi adresu DATA_ADDR
				DATA_EN <= '1';		-- povoli pracu s RAM
				DATA_RDWR <= '1';		-- aktualizuje hodnotu v DATA_RDATA

			when state_store_to_tmp1 =>
				next_state <= state_store_to_tmp2;
				help_reg_load_from_ram <= '1';	-- nastavi DATA_RDATA do help_reg
			
			when state_store_to_tmp2 =>
				tmp_reg <= help_reg(7 downto 0);		-- do tmp_reg nastavi help_reg
				pc_inc <= '1';
				next_state <= state_fetch0;
				
			---------------------------------------
			-- Nacita tmp_reg na adresu ptr, *ptr = tmp
			when state_load_from_tmp =>
				ptr_set <= '1';					-- nastavi DATA_ADDR
				tmp_reg_write_data <= '1';		-- do DATA_WDATA da tmp_reg
				DATA_EN <= '1';		-- povoli pracu s RAM
				DATA_RDWR <= '0';		-- aktualizuje data na DATA_ADDR a DATA_RDATA
				pc_inc <= '1';			-- ide na dalsiu instrukciu
				next_state <= state_fetch0;
			
			
			---------------------------------------
			---------------------------------------
			-- koncove stavy programu
			when state_end =>
				next_state <= state_end;
			
			when state_comment =>
				pc_inc <= '1';
				next_state <= state_fetch0;
				
			when others =>
				next_state <= state_end;
				
		end case;
	end process;


end behavioral;
 
