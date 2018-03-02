-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2017 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): xholop01
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
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='0') / zapis do pameti (DATA_RDWR='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

---------------------------=== SIGNALY ===-------------------------------------

  -- Finite state machine stavy procesoru
  type state is (
    --uvodne stavy
    fetch0,  -- prepare rom[pc]
    fetch1,  -- load rom[pc]
    decode,  -- decode instruction
    --stavy vykonavania
    exec_ptr_inc, -- ptr++
    exec_ptr_dec, -- ptr--
    exec_val_inc_1, --ram[ptr]
    exec_val_inc_2, --(*ptr)++
    exec_val_dec_1, --ram[ptr]
    exec_val_dec_2, --(*ptr)--
    --cyklus start
    exec_while_start, --pc++, ram[ptr]
    exec_while_do, --ram[ptr]==0 | cnt=1    
    exec_while_loop, --cnt==0 | rom[pc]                        
    exec_while_loop_inst, --load rom[pc]
    exec_test_if, --c=='[' cnt++ elsif ']' cnt--, pc++
    --cyklus end
    exec_while_end, --ram[ptr]    
    exec_while_do2, --ram[ptr]==0 pc | cnt=1, pc--     
    exec_while_do_t, --cnt==0, rom[pc]    
    exec_while_loop_inst2, -- load rom[pc]
    exec_test_if2, --c==']' cnt++ elsif '[' cnt--             
    exec_while_condition2, --c==0 pc++ else pc--
    --stavy vykonavania
    exec_write_0, --in_req=1    
    exec_write_1, --while(out_busy) (), store out_data ram[ptr], pc++
    exec_input, --while(!in_vld) (), store in_data, pc++
    exec_return, --cnt=1, pc++
    exec_return_if, --cnt==0
    exec_return_jump, --rom[pc]
    exec_return_inst, --c=='[' cnt++ elsif ']' cnt--, p++    
	 exec_nop, --pc=pc
    exec_halt --pc++
  );
  
  
  -- signaly podla navrhu procesoru v zadani
  -- stavy FSM
  signal current_state: state; -- CS
  signal next_state: state;  -- NS  	 
  -- zvysenie hodnot registrov
  signal pc_increase: std_logic;     --PC++
  signal ptr_increase: std_logic;    --PTR++
  signal cnt_increase: std_logic;    --CNT++
  -- znizenie hodnot registrov
  signal pc_decrease: std_logic;     --PC--
  signal ptr_decrease: std_logic;    --PTR--
  signal cnt_decrease: std_logic;    --CNT--
  -- samotne hodnoty
  signal pc_value: std_logic_vector(11 downto 0);  --PC  
  signal ptr_value: std_logic_vector(9 downto 0);  --PTR
  signal cnt_value: std_logic_vector(7 downto 0);  --CNT
  -- ostatne signaly 
  signal cnt_set: std_logic;   --CNT=1  
  signal load_instruction: std_logic;  --instruction=rom[PC]
  signal instruction: std_logic_vector(7 downto 0);  --instruction
  
    
---------------------------=== PROCESSES ===-----------------------------------

begin
 
  DATA_ADDR <= ptr_value;  --bugfix, aktualizacia data_addr  
    
  -- Samotny automat
  process_automat: process(CLK, RESET)
  begin
    -- pri asynchronnom resete zacni of fetch0
    if (RESET='1') then
      current_state <= fetch0;
    -- ak je aktivovany procesor, men stavy
    elsif (EN='1') and (CLK'event) and (CLK='1') then
      current_state <= next_state;
    -- inak nic nemen
	 else
		  current_state <= current_state;
    end if;
  end process process_automat;
  
  
  -- nastavenie hodnoty PC  
  process_pc: process(RESET, CLK)
  begin
    -- asynchronny reset
    if (RESET='1') then
      pc_value <= "000000000000";
    -- nastavenie PC podla signalov pri nastupnej hrane
    elsif (CLK'event) and (CLK='1') then
      -- zvysenie PC
      if (pc_increase = '1') then
        pc_value <= pc_value + 1;
      -- znizenie PC
      elsif (pc_decrease = '1') then
        pc_value <= pc_value - 1;
      end if;
    end if;
  end process process_pc;
  
  
  -- nastavenie hodnoty CNT
  process_cnt: process(RESET, CLK)
  begin
    -- asynchronnz reset
    if (RESET='1') then
      cnt_value <= "00000000";
    -- nastavenie CNT podla signalov pri nastupnej hrane
    elsif (CLK'event) and (CLK='1') then
      -- zvysenie CNT
      if (cnt_increase = '1') then
        cnt_value <= cnt_value + 1;
      -- znizenie CNT
      elsif (cnt_decrease = '1') then
        cnt_value <= cnt_value - 1;
      -- nastavenie CNT na jedna
      elsif (cnt_set = '1') then
        cnt_value <= "00000001";
      end if;
    end if;
  end process process_cnt;
  
  
  -- nastavenie hodnoty PTR  
  process_ptr: process(RESET, CLK)
  begin
    -- asynchronny reset
    if (RESET='1') then
      ptr_value <= "0000000000";
    -- nastavenie PTR podla signalov pri nastupnej hrane
    elsif (CLK'event) and (CLK='1') then
      -- zvysenie PTR
      if (ptr_increase = '1') then
        ptr_value <= ptr_value + 1;
      -- znizenie PTR
      elsif (ptr_decrease = '1') then
        ptr_value <= ptr_value - 1;
      end if;
    end if;
  end process process_ptr;   
    
  
  --nacitanie instrukcie
  process_load_instruction: process(CLK, RESET)
  begin
    --pri resete vynuluj instrukciu
    if (RESET='1') then
      instruction <= "00000000";
    --inak pri nastupnej hrane a signale pre nacitanie ju nacitaj
    elsif (CLK'event and CLK = '1') then
      if (load_instruction = '1') then
        instruction <= CODE_DATA;
      end if;
    end if;
  end process process_load_instruction;
  
  
  -- FSM procesoru
  --process_fsm: process(current_state, OUT_BUSY, DATA_RDATA, IN_DATA, CODE_DATA, IN_VLD, instruction, pc_value, ptr_value)
  process_fsm: process(CLK, RESET, current_state, pc_value, instruction, ptr_value, DATA_RDATA, OUT_BUSY, IN_VLD, IN_DATA, cnt_value)
  begin
    
    
    -- bugfix set_code_addr <= '0'; set_data_addr <= '0'; inc_data <= '0'; dec_data <= '0'; print_data <= '0';
    -- initializacia signalov pre pc, cnt, ptr	 
    pc_increase <= '0';
    pc_decrease <= '0';
    ptr_increase <= '0';
    ptr_decrease <= '0';
    cnt_increase <= '0';
    cnt_decrease <= '0';
    cnt_set <= '0';
    -- initializacia signalov pre ostatne prvky programu
    CODE_EN <= '0'; -- povolenie cinnosti k rom
    OUT_WE <= '0'; -- 1 pre vypis 
    DATA_EN <= '0'; -- povolenie cinnosti k ram
    IN_REQ <= '0'; -- vypis
    DATA_RDWR <= '0'; -- read/write
    load_instruction <= '0'; -- nacitanie instrukcie
    
    
    --definicie stavov
    case current_state is
    
      -- POMOCNE STAVY
      -- nacitanie a dekodovanie  
      
      --- FETCH0  ->  nastavenie pamate
       
      when fetch0 =>
          CODE_EN <= '1';
          CODE_ADDR <= pc_value;         
			    --bugfix, set_code_addr <= '1';
          next_state <= fetch1;
          
      --- FETCH1  ->  nacitanie instrukcie
      
      when fetch1 =>
          load_instruction <= '1';
          next_state <= decode;
                
      --- DECODE  -> dekodovanie instrukcie
      
        -- HINT
        -- 0x3E  ptr_val++
        -- 0x3C  ptr_val--
        -- 0x2B  *(ptr_val)++
        -- 0x2D  *(ptr_val)--
        -- 0x5B  while start
        -- 0x5D  while end
        -- 0x2E  print *(ptr_val)
        -- 0x2C  input *(ptr_val)
        -- 0x7E  break;
        -- 0x00  return;   
      --- 
                           
      when decode =>
          case instruction is
              when X"3E" => next_state <= exec_ptr_inc;
              when X"3C" => next_state <= exec_ptr_dec;
              when X"2B" => next_state <= exec_val_inc_1;
              when X"2D" => next_state <= exec_val_dec_1;
              when X"5B" => next_state <= exec_while_start;
              when X"5D" => next_state <= exec_while_end;
              when X"2E" => next_state <= exec_write_0;
              when X"2C" => next_state <= exec_input;
              when X"7E" => next_state <= exec_return;
              when X"00" => next_state <= exec_nop;
              when others => next_state <= exec_halt;
            end case;
      
      -- EXECUTE STAVY 
      -- samotne vykonavania dekodovanych instrukcii
      
      --- EXECUTE >
       
      when exec_ptr_inc =>
            ptr_increase <= '1';
            pc_increase <= '1';
            next_state <= fetch0;
      
      --- EXECUTE <
       
      when exec_ptr_dec =>
            ptr_decrease <= '1';
            pc_increase <= '1';
            next_state <= fetch0;
      
      --- EXECUTE +
       
      when exec_val_inc_1 =>
            DATA_EN <= '1';
            -- bugfix, DATA_ADDR <= ptr_value;
				-- bugfix, set_data_addr <= '1';
            DATA_RDWR <= '0';
            next_state <= exec_val_inc_2;
                  
      when exec_val_inc_2 =>
            DATA_EN <= '1';
            DATA_WDATA <= DATA_RDATA + 1;
				--bugfix, inc_data <= '1';
            DATA_RDWR <= '1';
            pc_increase <= '1';
            next_state <= fetch0;
        
      --- EXECUTE -
      
      when exec_val_dec_1 =>
            DATA_EN <= '1';
            --bugfix, DATA_ADDR <= ptr_value;
				--bugfix, set_data_addr <= '1';
            DATA_RDWR <= '0';
            next_state <= exec_val_dec_2;
            
      when exec_val_dec_2 =>
            DATA_EN <= '1';
            DATA_WDATA <= DATA_RDATA - 1;
				--bugfix, dec_data <= '1';
            DATA_RDWR <= '1';
            pc_increase <= '1';
            next_state <= fetch0;            
      
      -- CYKLY (aktualna verzia: jednoduche -> done, vnorene -> done)
      
      --- EXECUTE [
             
      when exec_while_start =>
			 -- PC <= PC + 1
          pc_increase <= '1';
			 -- RAM[PTR]
          DATA_EN <= '1';
          --bugfix, DATA_ADDR <= ptr_value;
          DATA_RDWR <= '0';
          next_state <= exec_while_do;
      
      when exec_while_do =>
			-- if (RAM[PTR] == 0)
         if (DATA_RDATA="00000000") then
           -- CNT <- 1
           cnt_set <= '1';
           next_state <= exec_while_loop;
          else
           next_state <= fetch0;
         end if;
      
      when exec_while_loop =>
        -- WHILE (CNT != 0)
        if (cnt_value="00000000") then
            next_state <= fetch0;
        else
           -- ROM[PC]
           CODE_EN <= '1';
           CODE_ADDR <= pc_value;
           next_state <= exec_while_loop_inst;
        end if;
           
      when exec_while_loop_inst =>
          -- C <= ROM[PC]
          load_instruction <= '1';
          next_state <= exec_test_if;
      
      when exec_test_if =>
          -- C == [
          if (instruction=X"5B") then
              -- CNT++
              cnt_increase <= '1';
          -- C == ]
          elsif (instruction=X"5D") then
              -- CNT--
              cnt_decrease <= '1';
          end if;
          -- PC = PC + 1
          pc_increase <= '1';
          next_state <= exec_while_loop;
                              
      
      --- EXECUTE ]
       
      when exec_while_end =>
        -- RAM[PTR]
        DATA_EN <= '1';
        --bugfix, DATA_ADDR <= ptr_value;
        DATA_RDWR <= '0';
        next_state <= exec_while_do2;
      
      when exec_while_do2 =>
          -- if RAM[PTR] == 0
          if (DATA_RDATA="00000000") then
              -- PC = PC + 1
              pc_increase <= '1';
              next_state <= fetch0;
          else
              -- CNT = 1
              cnt_set <= '1';
              -- PC = PC - 1
              pc_decrease <= '1';              
				  next_state <= exec_while_do_t;
          end if;
		
	  	when exec_while_do_t =>
          -- WHILE CNT!=0
  		    if (cnt_value="00000000") then
              next_state <= fetch0;
          else
              -- ROM[PC]
              CODE_EN <= '1';
              CODE_ADDR <= pc_value;
              next_state <= exec_while_loop_inst2;
          end if;
      
      when exec_while_loop_inst2 =>
          -- C <= ROM[PC]
          load_instruction <= '1';
          next_state <= exec_test_if2;
          
      when exec_test_if2 =>
          -- if C == ]
          if (instruction=X"5D") then
              -- CNT = CNT + 1
              cnt_increase <= '1';
          -- if C == [
          elsif (instruction=X"5B") then
              -- CNT = CNT - 1
              cnt_decrease <= '1';
          end if;
          next_state <= exec_while_condition2;
      
      when exec_while_condition2 =>
          --if CNT == 0
          if (cnt_value="00000000") then
              -- PC = PC + 1
             pc_increase <= '1';
          else
             -- PC = PC - 1     
			    pc_decrease <= '1';
          end if;
          next_state <= exec_while_do_t;
          
      
      --- EXECUTE .
      
      when exec_write_0 =>
          next_state <= exec_write_1;
          DATA_RDWR <= '0';
          --bugfix, DATA_ADDR <= ptr_value;
			 --bugfix, set_data_addr <= '1';
          DATA_EN <= '1';
           
      when exec_write_1 =>
          if (OUT_BUSY='1') then
              next_state <= exec_write_1;
          else
              --bugfix, DATA_EN<='1'; !!!
              OUT_DATA <= DATA_RDATA;
              OUT_WE <= '1';
              pc_increase <= '1';
              next_state <= fetch0;
          end if;
      
      --- EXECUTE ,
       
      when exec_input =>
          IN_REQ <= '1';
          if (IN_VLD='1') then
              DATA_EN <= '1';
              --bugfix, DATA_ADDR <= ptr_value;
				  --bugfix, set_data_addr <= '1';
              DATA_RDWR <= '1';
              DATA_WDATA <= IN_DATA;
              pc_increase <= '1';
              next_state <= fetch0;
				  IN_REQ <= '0';
          else 
              next_state <= exec_input;
          end if;
          
      --- EXECUTE ~
       
      when exec_return =>
          -- CNT = 1
          cnt_set <= '1';
          -- PC = PC + 1
          pc_increase <= '1';
          next_state <= exec_return_jump;
          
      when exec_return_jump =>
          -- while CNT != 0
          if (cnt_value="00000000") then
              next_state <= fetch0;
          else
              -- ROM[PC]
              CODE_EN <= '1';
              CODE_ADDR <= pc_value;
              next_state <= exec_return_inst;
          end if;
              
      when exec_return_inst =>
          -- C = rom[pc] 
          load_instruction <= '1';
          next_state <= exec_return_if;
          
      when exec_return_if =>
          -- c == [
          if (instruction=X"5B") then
              --CNT++
              cnt_increase <= '1';
          -- c == ]
          elsif (instruction=X"5D") then
              --CNT--
              cnt_decrease <= '1';
          end if;
          --PC = PC + 1
          pc_increase <= '1';
          next_state <= exec_return_jump;   
                 
      --- EXECUTE null -> end of program
           
      when exec_nop =>
          next_state <= exec_nop;
		
  		--- EXECUTE ine instrukcie
      
	   when exec_halt =>
			 pc_increase <= '1';
			 next_state <= fetch0;
		
      --- EXECUTE ine stavy automatu (pre istotu)
    
		when others =>
			 pc_increase <= '1';
			 next_state <= fetch0;
        
      end case;
      
  end process process_fsm;    
  

end behavioral;
 
