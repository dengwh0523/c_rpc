do
    --协议名称为Rmi，在Packet Details窗格显示为rmi
    local rmi_proto = Proto("Rmi2","rmi protocol")
    --协议的各个字段
    local f_magic = ProtoField.string("Rmi2.magic","Magic")
    local f_len = ProtoField.uint32("Rmi2.len","Length",base.DEC)
    local f_seq = ProtoField.uint32("Rmi2.seq","Sequence",base.DEC)
    local f_ver = ProtoField.uint8("Rmi2.ver","Version",base.DEC)
    local f_opt = ProtoField.uint8("Rmi2.opt","Option",base.DEC)
    local f_mtype = ProtoField.uint8("Rmi2.mtype","Operation",base.DEC,
	{[1] = "REQ1", [2] = "REQ2", [3] = "RESPONSE", [4] = "NOTIFY", [5] = "CLOSE"})
    local f_stat = ProtoField.uint8("Rmi2.stat","Status",base.DEC,
	{[0] = "success", [1] = "no such object", [2] = "no such interface", [3] = "no such method"})
	local f_funcid = ProtoField.uint32("Rmi2.funcid", "Function ID", base.HEX)
	local f_crc = ProtoField.uint32("Rmi2.crc", "Crc", base.HEX)
	
    rmi_proto.fields = {f_magic,f_len,f_seq,f_ver,f_opt,f_mtype,f_stat,f_funcid,f_crc}
    
    local data_dis = Dissector.get("data")
    
    local function Rmi_dissector(buf,pkt,root)
        local buf_len = buf:len()
        --先检查报文长度，太短的不是我的协议
        if buf_len < 24 then return false end
        --取得前4字节magic字段的值,验证magic是否正确
        if ((buf(0,1):uint()~=114) or (buf(1,1):uint()~=109) or (buf(2,1):uint()~=105)
            or (buf(3,1):uint()~=0))
            --不正确就不是我的协议
            then return false end
			
		-- get magic
		local v_magic = buf(0, 4)
        -- --取得operator的值
        -- local v_len = buf(4,4)
        -- local i_len = v_len:uint()
		
		-- -- get seq value
        -- local v_seq = buf(8,4)
        -- local i_seq = v_seq:uint()
		
		-- -- get ver value
        -- local v_ver = buf(12,1)
        -- local i_ver = v_ver:uint()
		
		-- -- get opt value
        -- local v_opt = buf(13,1)
        -- local i_opt = v_opt:uint()
        
        --现在知道是我的协议了，放心大胆添加Packet Details
        local t = root:add(rmi_proto,buf(0, 24))
        --在Packet List窗格的Protocol列也可以“做个小广告”
        pkt.cols.protocol = "Rmi2"
        t:add(f_magic, buf(0, 4))
        t:add(f_len, buf(4, 4))
        t:add(f_seq, buf(8, 4))
        t:add(f_ver, buf(12, 1))
        t:add(f_opt, buf(13, 1))
        t:add(f_mtype, buf(14, 1))
        t:add(f_stat, buf(15, 1))
        t:add(f_funcid, buf(16, 4))
        t:add(f_crc, buf(20, 4))
        return true
    end
    
    function rmi_proto.dissector(buf,pkt,root) 
        if Rmi_dissector(buf,pkt,root) then
            --valid Rmi2 diagram
			data_dis:call(buf(24):tvb(), pkt, root)
        else
            --data这个dissector几乎是必不可少的；当发现不是我的协议时，就应该调用data
            data_dis:call(buf,pkt,root)
        end
    end
    
    local tcp_encap_table = DissectorTable.get("tcp.port")
    --只需要处理TCP 9999端口就可以了
    tcp_encap_table:add(9999, rmi_proto)
end