-- author: Patrik Holop, xholop01@stud.fit.vutbr.cz

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;


-- Jedina entita reprezentujuca display
entity ledc8x8 is
port (
  -- vektor lediek
  LED : out std_logic_vector(0 to 7);
  -- vektor riadkov
  ROW : out std_logic_vector(0 to 7);
  -- reset 
  RESET : in std_logic;
  -- pre nastupnu hranu
  SMCLK : in std_logic
);
end ledc8x8;


-- Architektura
architecture main of ledc8x8 is
  -- pole pre jednoduchy citac
  signal cnt: std_logic_vector(7 downto 0);   
  -- logicky "vystup" citacu 
  signal ce: std_logic;
  -- signal pre ledky
  signal LEDS: std_logic_vector(7 downto 0);
  -- signal pre riadky
  signal ROWS: std_logic_vector(7 downto 0);


-- samotne procesy  
begin

  -- citac pre znizenie frekvencie zasielania instrukcii do LCD   
  -- prednaska 03 fitkit 
  ce_gen: process(SMCLK, RESET)
  begin
    -- asynchronny reset
    if RESET = '1' then
      cnt <= "00000000";
    -- zvys citac
    elsif SMCLK'event and SMCLK = '1' then
      cnt <= cnt + 1;
    end if;
  end process ce_gen;
  -- vyhodnot citac
  ce <= '1' when cnt = "11111111" else '0';

  
  -- zrotovanie riadkov
  rotuj: process(ce, SMCLK, RESET, ROWS)
  begin
    -- asynchronny reset
    if RESET = '1' then
      ROWS <= "00000001";
    -- posunutie riadku v pripade pokynu citaca
    elsif SMCLK'event and SMCLK = '1' and ce = '1' then
      case ROWS is
        when "00000001" => ROWS <= "00000010";
        when "00000010" => ROWS <= "00000100";
        when "00000100" => ROWS <= "00001000";
        when "00001000" => ROWS <= "00010000";
        when "00010000" => ROWS <= "00100000";
        when "00100000" => ROWS <= "01000000";
        when "01000000" => ROWS <= "10000000";
        when "10000000" => ROWS <= "00000001";
        when others => ROWS <= "00000001";
      end case;
    end if;
  end process rotuj;
  
  
  --dekoder pre urcenie, co sa ma zobrazit na jednotlivych riadkoch
  dekoder: process(ROWS)
  begin
    case ROWS is
      when "00000001" => LEDS <= "11110110";
      when "00000010" => LEDS <= "11110110";
      when "00000100" => LEDS <= "01110000";
      when "00001000" => LEDS <= "01110110";
      when "00010000" => LEDS <= "01110110";
      when "00100000" => LEDS <= "00001111";
      when "01000000" => LEDS <= "01101111";
      when "10000000" => LEDS <= "00001111";
      when others => LEDS <= "11111111";
    end case;
  end process dekoder;
  
  --samotne zobrazenie
  show: process(ROWS, LEDS)
  begin
    ROW <= ROWS;
    LED <= LEDS;
  end process show;
  
end main;
