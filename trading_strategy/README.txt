1. Strategy Implementation:
	a. Language: C++
	b. Folders:
		# Documentation is available in "documentation" folder
		# Raw backtesting data needs to be stored in "data"  folder 
		# Log file will be written in "logs" folder
		# High-level P&L results will be written in "results" folder
	c. Functionality: 
		# There are 2 strategies (GapDownFadeUp and GapUpFadeDown) implemented which use same backtesting data. Details in "StrategyDetails.pdf"
		# The strategy is executable and can be backtested with historical market data

2. Download Backtesting data:
	a. Download Dukascopy data (using Tickstory or JForex platform)
	b. Backtesting code can handle both "tick" and "1 second" data
		# format of download mentioned in ./data/DUKASCOPY_format.txt file
	c. Expected filename is: XXXXXX_MMMYYYY_type.csv, where
		# XXXXXX is the currency pair (e.g. USDJPY, EURUSD)
		# MMMYYYY is the date part (e.g. May2024, Feb2024)
		# type is the data type (i.e. 1s for "per second data" and "t" for tick data)
	d. Some example extracts are provided in the "data" folder

3. Setup, Build and Run Backtesting:
	a. Download code from GitHub and maintain the same directory structure as in the repository
	b. Currency setup:
		# The code is pre-setup to handle USDJPY and EURUSD data. 
		# To add other currency pairs: add to ccyPairs enum in datamodel.h; create new CcyPairDef in main.cpp; add to ccies vector in main.cpp
	c. Compile a Release build
	d. Ensure that backtesting data is available in "data" folder
	e. Run executable after compilation is complete
	
4. Results & Log files
	a. Trade log results will available in "logs" folder:
		# Log filename format is: tradeLog_XXXXXX_MMMYYYY_type_timestamp.txt (e.g. tradeLog_EURUSD_Apr2024_1s_1719630601.txt)
	b. Each chunk in the log file contains:
		# which strategy is being executed (i.e. GapDownFadeUp or GapUpFadeDown)
		# details of a NEW trade (after strategy has followed GapUp/GapDown trend and identifes the "Fade") 
		# details of market data moves after NEW trade is initiated and before a close-out event is triggered
		# details of the Close-out (TAKEPROFIT, STOPLOSS and KILL) event and trade
	c. P&L results will available in "results" folder:
		# P&L filename format is: pnl_XXXXXX_MMMYYYY_type_timestamp.txt (e.g. pnl_EURUSD_Apr2024_1s_1719630601.txt)
		# File has results per strategy (GapDown and GapUp) with breakdown per close out type (TakeProfit, StopLoss, Kill) 
		# Results are (i) total P&L and (ii) number of trades per close out type per Strategy.
	d. The P&L results filename and Log filename have the same "timestamp" extension, so that it's easy to pair them up.