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
      part = full_parts.select{|b|b.machine_id == mac[0] && cycle_start != nil}     
      times = Machine.new_run_time(logs, full_logs, start_time, Time.now.localtime)
      duration = (end_time.to_i - start_time.to_i).to_i
      run = times[:run_time]
      idle = times[:idle_time]
      stop = times[:stop_time]
      utilization = (run*100)/duration

      byebug
    end
  end
end
