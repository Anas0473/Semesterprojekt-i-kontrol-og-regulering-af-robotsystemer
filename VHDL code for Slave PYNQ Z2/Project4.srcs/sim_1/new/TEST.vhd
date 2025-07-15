library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity SPI_SLAVE_tb is
end entity;

architecture testbench of SPI_SLAVE_tb is
    -- Component Declaration
    component SPI_SLAVE is
        Generic (
            WORD_SIZE : natural := 16
        );
        Port (
            CLK      : in  std_logic;
            SCLK     : in  std_logic;
            CS_N     : in  std_logic;
            MOSI     : in  std_logic;
            MISO     : out std_logic;
            hallsensor1A, hallsensor1B, hallsensor2A, hallsensor2B, sensor0, sensor1 : in std_logic;
            IN1A, IN2A, IN1B, IN2B : out std_logic;
            ENA, ENB : out std_logic
        );
    end component;

    -- Signals for SPI_SLAVE inputs and outputs
    signal CLK : std_logic := '0';
    signal SCLK : std_logic := '0';
    signal CS_N : std_logic := '1';
    signal MOSI : std_logic := '0';
    signal MISO : std_logic;
    signal hallsensor1A, hallsensor1B, hallsensor2A, hallsensor2B : std_logic := '0';
    signal sensor0, sensor1 : std_logic := '0';
    signal IN1A, IN2A, IN1B, IN2B : std_logic;
    signal ENA, ENB : std_logic;

    -- Clock period definitions
    constant CLK_PERIOD : time := 10 ns;

begin
    -- Instantiate the SPI_SLAVE component
    uut: SPI_SLAVE
        generic map (
            WORD_SIZE => 16
        )
        port map (
            CLK => CLK,
            SCLK => SCLK,
            CS_N => CS_N,
            MOSI => MOSI,
            MISO => MISO,
            hallsensor1A => hallsensor1A,
            hallsensor1B => hallsensor1B,
            hallsensor2A => hallsensor2A,
            hallsensor2B => hallsensor2B,
            sensor0 => sensor0,
            sensor1 => sensor1,
            IN1A => IN1A,
            IN2A => IN2A,
            IN1B => IN1B,
            IN2B => IN2B,
            ENA => ENA,
            ENB => ENB
        );

    -- Clock process definitions
    CLK_process :process
    begin
        CLK <= '0';
        wait for CLK_PERIOD/2;
        CLK <= '1';
        wait for CLK_PERIOD/2;
    end process;

    -- Simulation process
    stim_proc: process
    begin
        -- Reset
        hallsensor2A <= '0';
        hallsensor2B <= '0';
        wait for 20 ns;
        
        -- Simulate hallsensor2B rising edge
        for i in 0 to 300 loop
            hallsensor2A <= '1';
            wait for 10 ns;
            hallsensor2B <= '1';
            wait for 10 ns;
            hallsensor2B <= '0';
            hallsensor2A <= '0';
            wait for 20 ns;
        end loop;
        
        -- Finish simulation
        wait;
    end process;

end architecture;
