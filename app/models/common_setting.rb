class CommonSetting < ApplicationRecord
  enum type: {"Tenant-Setting": 1, "User-Setting": 2, "Machine-Setting": 3}
  enum machine_type: {"Ethernet": 1, "Rs232": 2, "Ct/Pt": 3}

  def self.part(tenant, shift_no, date)

  	tenant = Tenant.find(tenant)
  	shifts = Shifttransaction.includes(:shift).where(shifts: {tenant_id: tenant})
    shift = shifts.find_by_shift_no(shift_no)
       
    case
    when shift.day == 1 && shift.end_day == 1
      start_time = (date+" "+shift.shift_start_time).to_time
      end_time = (date+" "+shift.shift_end_time).to_time
    when shift.day == 1 && shift.end_day == 2
      start_time = (date+" "+shift.shift_start_time).to_time
      end_time = (date+" "+shift.shift_end_time).to_time+1.day
    else
      start_time = (date+" "+shift.shift_start_time).to_time+1.day
      end_time = (date+" "+shift.shift_end_time).to_time+1.day
    end
        
    tenant.machines.where(controller_type: 1).order(:id).map do |mac|
    machine_log = mac.machine_daily_logs.where("created_at >=? AND created_at <?",start_time,end_time).order(:id)
      machine_log.each_with_index do |log, index|
        if log.machine_status == 100  
        else	
         	if Machine.find(mac.id).parts.present?
         		last_part = mac.parts.last
         		if last_part.part.to_i == log.parts_count.to_i && last_part.program_number.to_i == log.programe_number.to_i
         		else
         		cutt = (log.total_cutting_time.to_i * 60) + (log.total_cutting_second.to_i / 1000)
         		
         		cutt_time = cutt - last_part.cutting_time.to_i
         		cycle_time = (log.run_time.to_i * 60) + (log.run_second.to_i / 1000)
         		
         		mac.parts.last.update(cutting_time: cutt_time, cycle_time: cycle_time, part_end_time: log.created_at)
         		Part.create(date: log.created_at, shift_no: nil, part: log.parts_count, program_number: log.programe_number, cycle_time: nil, cutting_time: cutt, cycle_st_to_st: nil, cycle_stop_to_stop: nil, time: nil, part_start_time: log.created_at, part_end_time: log.created_at, cycle_start: nil, status: 1, is_active: true, deleted_at: nil, shifttransaction_id: nil, machine_id: mac.id)
         	  end
         	else
         		cutt = (log.total_cutting_time.to_i * 60) + (log.total_cutting_second.to_i / 1000)
         		Part.create(date: log.created_at, shift_no: nil, part: log.parts_count, program_number: log.programe_number, cycle_time: nil, cutting_time: cutt, cycle_st_to_st: nil, cycle_stop_to_stop: nil, time: nil, part_start_time: log.created_at, part_end_time: log.created_at, cycle_start: nil, status: 1, is_active: true, deleted_at: nil, shifttransaction_id: nil, machine_id: mac.id)
         	end
        end
      end
    end
  end


  def self.report(tenant, shift_no, date)
    @alldata = []
    tenant = Tenant.find(tenant)
    shifts = Shifttransaction.includes(:shift).where(shifts: {tenant_id: tenant})
    shift = shifts.find_by_shift_no(shift_no)
    case
    when shift.day == 1 && shift.end_day == 1
      start_time = (date+" "+shift.shift_start_time).to_time
      end_time = (date+" "+shift.shift_end_time).to_time
    when shift.day == 1 && shift.end_day == 2
      start_time = (date+" "+shift.shift_start_time).to_time
      end_time = (date+" "+shift.shift_end_time).to_time+1.day
    else
      start_time = (date+" "+shift.shift_start_time).to_time+1.day
      end_time = (date+" "+shift.shift_end_time).to_time+1.day
    end

    machine_ids = Tenant.find(tenant).machines.where(controller_type: 1).pluck(:id,:machine_name)
    mac_id = machine_ids.map{|i| i[0]}
    full_logs = MachineDailyLog.where(machine_id: mac_id)
    full_parts = Part.where(date: date, shift_no:shift_no, machine_id: mac_id)
    
machine_ids.each do |mac|
      machine_log = full_logs.select{|a| a.machine_id == mac[0]}
      logs = machine_log.select{|a| a.created_at > start_time && a.created_at < end_time }
      part = full_parts.select{|b|b.machine_id == mac[0] && b.cycle_start != nil}     
      times = Machine.new_run_time(logs, full_logs, start_time, end_time)
      duration = (end_time.to_i - start_time.to_i).to_i
      run = times[:run_time]
      idle = times[:idle_time]
      stop = times[:stop_time]
      utilization = (run*100)/duration
      
       cycle_st_to_st = []
       cutting_time = []
       cycle_stop_to_stop = []
       cycle_time = []
      
      part.each do |p|
       cycle_time << {program_number: p.program_number, cycle_time: p.cycle_time.to_i, parts_count: p.part.to_i} 
       cutting_time << p.cutting_time.to_i
       cycle_st_to_st << p.cycle_start.to_time
       cycle_stop_to_stop << p.part_end_time.to_time
      end
     
        
      axis_loadd = []
      tempp_val = []
      puls_coder = []

      if logs.present?
        feed_rate_max = logs.pluck(:feed_rate).reject{|i| i == "" || i.nil? || i > 5000 || i == 0 }.map(&:to_i).max
        spindle_speed_max = logs.pluck(:cutting_speed).reject{|i| i == "" || i.nil? || i == 0 }.map(&:to_i).max
        sp_temp_min = logs.pluck(:z_axis).reject{|i| i == "" || i.nil? || i == 0 }.map(&:to_i).min
        sp_temp_max = logs.pluck(:z_axis).reject{|i| i == "" || i.nil? || i == 0 }.map(&:to_i).max
        spindle_load_min = logs.pluck(:spindle_load).reject{|i| i == "" || i.nil? || i == 0 }.map(&:to_i).min
        spindle_load_max = logs.pluck(:spindle_load).reject{|i| i == "" || i.nil? || i == 0 }.map(&:to_i).max
      
	mac_setting_id =  MachineSetting.find_by(machine_id: mac_id).id     
        data_val = MachineSettingList.where(machine_setting_id: mac_setting_id, is_active: true).pluck(:setting_name)

            logs.last.x_axis.first.each_with_index do |key, index|
              if data_val.include?(key[0].to_s)
                load_value =  logs.pluck(:x_axis).sum.pluck(key[0]).reject{|i| i == "" || i.nil? || i == 0 }.map(&:to_i).min.to_s+' - '+logs.pluck(:x_axis).sum.pluck(key[0]).reject{|i| i == "" || i.nil? || i == 0 }.map(&:to_i).max.to_s
                temp_value =  logs.pluck(:y_axis).sum.pluck(key[0]).reject{|i| i == "" || i.nil? || i == 0 }.map(&:to_i).min.to_s+' - '+logs.pluck(:y_axis).sum.pluck(key[0]).reject{|i| i == "" || i.nil? || i == 0 }.map(&:to_i).max.to_s
                puls_value =  logs.pluck(:cycle_time_minutes).sum.pluck(key[0]).reject{|i| i == "" || i.nil? || i == 0 }.map(&:to_i).min.to_s+' - '+logs.pluck(:cycle_time_minutes).sum.pluck(key[0]).reject{|i| i == "" || i.nil? || i == 0 }.map(&:to_i).max.to_s
                
                if load_value == " - "
                  load_value = "0 - 0" 
                end

                if temp_value == " - "
                  temp_value = "0 - 0" 
                end

                if puls_value == " - "
                  puls_value = "0 - 0" 
                end
              
                axis_loadd << {key[0].to_s.split(":").first => load_value}
                tempp_val << {key[0].to_s.split(":").first => temp_value}
                puls_coder << {key[0].to_s.split(":").first => puls_value}
              else
                axis_loadd << {key[0].to_s.split(":").first => "0 - 0"}
                tempp_val <<  {key[0].to_s.split(":").first => "0 - 0"}
                puls_coder << {key[0].to_s.split(":").first => "0 - 0"}
              end
            end


      else
       feed_rate_max = 0
       spindle_speed_max = 0
       sp_temp_min = 0
       sp_temp_max = 0
       spindle_load_min = 0
       spindle_load_max = 0
      end




      if shift.operator_allocations.where(machine_id: mac[0]).last.nil?
        operator_id = nil
        target = 0
      else
        if shift.operator_allocations.where(machine_id: mac[0]).present?
         shift.operator_allocations.where(machine_id: mac[0]).each do |ro|
         aa = ro.from_date
         bb = ro.to_date
         cc = date
          if cc.to_date.between?(aa.to_date,bb.to_date)
            dd = ro#cc.to_date.between?(aa.to_date,bb.to_date)
            if dd.operator_mapping_allocations.where(:date=>date.to_date).last.operator.present?
              operator_id = dd.operator_mapping_allocations.where(:date=>date.to_date).last.operator.id
              target = dd.operator_mapping_allocations.where(:date=>date.to_date).last.target
            else
              operator_id = nil
              target = 0
            end
          else
            operator_id = nil
            target = 0
          end
         end
        else
          operator_id = nil
          target = 0
        end
       end
      
     
       @alldata << [
                 
        date,
        start_time.strftime("%H:%M:%S")+' - '+end_time.strftime("%H:%M:%S"),
        duration,
        shift.shift.id,
        shift.shift_no,
        operator_id,#(operator)
        mac[0], #(machine_id)
        "test",
        cutting_time.count,
        run,
        idle,
        stop,
        0,
        logs.count,
        utilization,
        tenant.id,
        cycle_time,
        cycle_st_to_st,
        feed_rate_max.to_s,
        spindle_speed_max.to_s,
        part.count,
        0,
        0,
        0,
        0,
        0,
        cycle_stop_to_stop,
        cutting_time,
        spindle_load_min.to_s+' - '+spindle_load_max.to_s,
        sp_temp_min.to_s+' - '+sp_temp_max.to_s,
        axis_loadd,
        tempp_val,
        puls_coder,
        0,
        0,
	0                          
       ]

end


if @alldata.present?

      @alldata.each do |data|
        if CncReport.where(date:data[0],shift_no: data[4], time: data[1], machine_id:data[6], tenant_id:data[15]).present?
          CncReport.find_by(date:data[0],shift_no: data[4], time: data[1], machine_id:data[6], tenant_id:data[15]).update(date:data[0], time: data[1], hour: data[2], shift_id: data[3], shift_no: data[4], operator_id: data[5], machine_id: data[6], job_description: data[7], parts_produced: data[8], run_time: data[9], idle_time: data[10], stop_time: data[11], time_diff: data[12], utilization: data[14],  tenant_id: data[15], all_cycle_time: [], cycle_start_to_start: data[17], feed_rate: data[18], spendle_speed: data[19], data_part: data[20], target:data[21], approved: data[23], rework: data[24], reject: data[25], stop_to_start: data[26], cutting_time: data[27],spindle_load: data[28], spindle_m_temp: data[29], servo_load: data[30], servo_m_temp: data[31], puls_code: data[32],availability: 0, perfomance: data[34], quality: data[35], parts_data: data[16])
        else
          CncReport.create!(date:data[0], time: data[1], hour: data[2], shift_id: data[3], shift_no: data[4], operator_id: data[5], machine_id: data[6], job_description: data[7], parts_produced: data[8], run_time: data[9], idle_time: data[10], stop_time: data[11], time_diff: data[12], utilization: data[14],  tenant_id: data[15], all_cycle_time: [], cycle_start_to_start:data[17], feed_rate: data[18], spendle_speed: data[19], data_part: data[20], target:data[21], approved: data[23], rework: data[24], reject: data[25], stop_to_start: data[26], cutting_time: data[27],spindle_load: data[28], spindle_m_temp: data[29], servo_load: data[30], servo_m_temp: data[31], puls_code: data[32],availability: data[33], perfomance: data[34], quality: data[35],parts_data: data[16])
        end
      end
    end






end
end
