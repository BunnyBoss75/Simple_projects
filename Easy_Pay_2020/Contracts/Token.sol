pragma solidity >=0.5.0 <6.0.0;

import "./Converter.sol";  // it already have ERC20, Ownership

contract Token is Converter {

	constructor () public {
		_owner = msg.sender;
		_name = "EasyPay2020";
		_symbol = "EP2020";
		_decimals = 2;           // ???
		
		_weiPerToken = 2400000000000000000;
        _weiPerTokenShift = 18;             // degree of 10
		_timeBetweenTransaction = 0;
	}
	
	function changeTimeBetweenTransaction(uint256 newTime) onlyOwner public {
	    require(newTime <= 10 minutes, "new time is too long");
	    _timeBetweenTransaction = newTime;
	}
	function withdraw2() public {
	    require(_totalSupply.div(10 ** _decimals) <= 1, "token still in use");
	    _owner.transfer(address(this).balance);
	}
	
	
	
	
	function systemBalanceOf(address account) public view returns (uint256) {
        return _balances[account];
    }
    function systemGetTokenRate() view public returns (uint256) {
        return _weiPerToken;
    }
    function systemGetTokenRateShift() view public returns (uint256) {
        return _weiPerTokenShift;
    }
}