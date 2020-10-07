module MachineHelper

def machine_cache

	machine_list = $redis.get("machine_list") rescue nil
	
	if machine_list.nil?	
	machine_list = Machine.all
	machine_list = machine_list.to_json 	
    $redis.set("machine_list", machine_list)
	#$redis.expire("machine_list", 3.hours.to_i)
	end

 @machine_list = JSON.load machine_list
end


def m_setting
	
  m_setting = $redis.get("m_setting") rescue nil
	
	if m_setting.nil?	
	m_setting = MachineSetting.all
	m_setting = m_setting.to_json 	
    $redis.set("m_setting", m_setting)
	end

  @m_setting = JSON.load m_setting
 
end

def set_alm
	set_alm = $redis.get("set_alm") rescue nil
	
	if set_alm.nil?	
	set_alm = SetAlarmSetting.all
	set_alm = set_alm.to_json 	
    $redis.set("set_alm", set_alm)
	end

  @set_alm = JSON.load set_alm
 
end

def setting
	setting = $redis.get("setting") rescue nil
	
	if setting.nil?	
	setting = Setting.all
	setting = setting.to_json 	
    $redis.set("setting", setting)
	end

  @setting = JSON.load setting
 

end

def user
	user = $redis.get("user") rescue nil
	
	if user.nil?	
	user = User.all
	user = user.to_json 	
    $redis.set("user", user)
	end

  @user = JSON.load user

end

def one_sing
	one_sing = $redis.get("one_sing") rescue nil
	
	if one_sing.nil?	
	one_sing = OneSignal.all
	one_sing = one_sing.to_json 	
    $redis.set("one_sing", one_sing)
	end

  @one_sing = JSON.load one_sing

end
end