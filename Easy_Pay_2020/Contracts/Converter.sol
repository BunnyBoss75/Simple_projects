pragma solidity >=0.5.0 <6.0.0;

import "./ERC20.sol";
import "./Ownership.sol";

contract Converter is ERC20, Ownership {
    
    uint256 _weiPerToken;
    uint256 _weiPerTokenShift;
    
    function changeWeiPerToken(uint256 coef) onlyOwner public {
        require(coef != 0, "coef is 0");
        _weiPerToken = coef;
    }
    function changeWeiPerTokenShift(uint256 coef) onlyOwner public {
        require(coef != 0, "coef is 0");
        _weiPerTokenShift = coef;
    }
    
    
    
    function getTokenRate() view public returns (uint256) {    // problem: rounding down
        return _weiPerToken.div(10 ** _weiPerTokenShift);
    }
    
    function convertEtherToToken() public payable returns (bool) {
        _canSendTransaction(msg.sender);
        _timeCanSendTransaction[msg.sender] = now + _timeBetweenTransaction;
        
        _mint(msg.sender, msg.value.mul(10 ** _weiPerTokenShift).mul(10 ** _decimals).div(_weiPerToken));    // with decimals and shift
        
        return true;
    }
    function convertTokenToEther(uint256 amount) public payable returns (bool) {
        _canSendTransaction(msg.sender);
        _timeCanSendTransaction[msg.sender] = now + _timeBetweenTransaction;
        
        address payable recipient = msg.sender;
        uint256 amountWei = amount.mul(_weiPerToken).div(10 ** _weiPerTokenShift);
        require(address(this).balance >= amountWei, "Contract don't have enough wei");
        
        _burn(msg.sender, amount.mul(10 ** _decimals));
        recipient.transfer(amountWei);
        
        return true;
    }
    
    
    
    function _mint(address account, uint256 amount) internal {                       // already with decimals
        require(account != address(0), "ERC20: mint to the zero address");
        _totalSupply = _totalSupply.add(amount);
        _balances[account] = _balances[account].add(amount);
        emit Transfer(address(0), account, amount);
    }
    function _burn(address account, uint256 amount) internal {                      // already with decimals
        require(account != address(0), "ERC20: burn from the zero address");
        require(_balances[account] >= amount, "You don't have enough token");                          //not need, for show error
        _balances[account] = _balances[account].sub(amount);
        _totalSupply = _totalSupply.sub(amount);
        emit Transfer(account, address(0), amount);
    }
}