-- fsm.vhd: Finite State Machine
-- Author(s): xholop01 PatrikHolop
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST1, TEST2, TEST3, TEST4, TEST5, TEST6, TEST7, TEST8a,
   TEST8b, TEST9a, TEST9b, TEST10a, TEST10b, TEST11a, TEST11b, PRINT_SUCCESS, PRINT_ERROR, 
   SUCCESS, ERROR, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- STAVY KONCOVE
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
       next_state <= FINISH;
       if (KEY(15) = '1') then  -- opakovanie hesla
           next_state <= TEST1;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= FINISH; 
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - - 
   when SUCCESS =>
       next_state <= SUCCESS; 
       if (KEY(15) = '1') then  -- potvrdenie hesla
           next_state <= PRINT_SUCCESS; 
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- vacsi pocet znakov hesla
           next_state <= ERROR;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - - 
   when ERROR =>
       next_state <= ERROR;
       if (KEY(15) = '1') then -- potvrdenie nespravneho hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- pokracovanie v hesle
           next_state <= ERROR; 
       end if;
      
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- TESTOVACIE STAVY
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1 =>
       next_state <= TEST1;
       if (KEY(15) = '1') then  -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(1) = '1') then -- spravna klavesa
           next_state <=TEST2;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= ERROR; 
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST2 =>
       next_state <=TEST2;
       if (KEY(15) = '1') then -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(4) = '1') then -- spravna klavesa
           next_state <= TEST3;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= ERROR;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST3 =>
       next_state <=TEST3;
       if (KEY(15) = '1') then -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(0) = '1') then -- spravna klavesa
           next_state <= TEST4;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= ERROR;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST4 =>
       next_state <=TEST4;
       if (KEY(15) = '1') then -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(8) = '1') then -- spravna klavesa
           next_state <= TEST5;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= ERROR;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST5 =>
       next_state <=TEST5;
       if (KEY(15) = '1') then -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(3) = '1') then -- spravna klavesa
           next_state <= TEST6;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= ERROR;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST6 =>
       next_state <=TEST6;
       if (KEY(15) = '1') then -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(8) = '1') then -- spravna klavesa
           next_state <= TEST7;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= ERROR;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST7 =>
       next_state <=TEST7;
       if (KEY(15) = '1') then -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(3) = '1') then  -- spravna klavesa
           next_state <= TEST8a;
       elsif (KEY(6) = '1') then  -- spravna klavesa
           next_state <= TEST8b;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= ERROR;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST8a =>
       next_state <=TEST8a;
       if (KEY(15) = '1') then -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(1) = '1') then  -- spravna klavesa
           next_state <= TEST9a;
       elsif (KEY(15 downto 0) /= "0000000000000000") then  -- ina klavesa
           next_state <= ERROR;
       end if;   
  -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST9a =>
       next_state <=TEST9a;
       if (KEY(15) = '1') then -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(1) = '1') then  -- spravna klavesa
           next_state <= TEST10a;
       elsif (KEY(15 downto 0) /= "0000000000000000") then  -- ina klavesa
           next_state <= ERROR;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST10a =>
       next_state <=TEST10a;
       if (KEY(15) = '1') then -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(6) = '1') then  -- spravna klavesa
           next_state <= TEST11a;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= ERROR;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST11a =>
       next_state <=TEST11a;
       if (KEY(15) = '1') then -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(8) = '1') then  -- spravne HESLO
           next_state <= SUCCESS;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= ERROR;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST8b =>
       next_state <=TEST8b;
       if (KEY(15) = '1') then -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(4) = '1') then  -- spravna klavesa
           next_state <= TEST9b;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= ERROR;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST9b =>
       next_state <=TEST9b;
       if (KEY(15) = '1') then -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(8) = '1') then  -- spravna klavesa
           next_state <= TEST10b;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= ERROR;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST10b =>
       next_state <=TEST10b;
       if (KEY(15) = '1') then -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(9) = '1') then  -- spravna klavesa
           next_state <= TEST11b;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= ERROR;
       end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST11b =>
       next_state <=TEST11b;
       if (KEY(15) = '1') then  -- skore potvrdenie hesla
           next_state <= PRINT_ERROR;
       elsif (KEY(1) = '1') then  -- spravne HESLO
           next_state <= SUCCESS;
       elsif (KEY(15 downto 0) /= "0000000000000000") then -- ina klavesa
           next_state <= ERROR;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- VYPIS STAVY
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_SUCCESS =>
       next_state <= PRINT_SUCCESS;
       if (CNT_OF = '1') then
           next_state <= FINISH;
       end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_ERROR =>
       next_state <= PRINT_ERROR;
       if (CNT_OF = '1') then
           next_state <= FINISH;
       end if;

  --------------------------------------------------
  
   when others =>
      next_state <= TEST1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_SUCCESS =>
      FSM_MX_MEM     <= '1';
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_ERROR =>
      FSM_MX_MEM     <= '0';
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
   -- INE STAVY (testovacie)
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   end case;
end process output_logic;

end architecture behavioral;

