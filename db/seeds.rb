Companytype.create!([
  {companytype_name: "Altius", description: nil}
])

Usertype.create!([
  {usertype_name: "Altius-client", description: nil},
  {usertype_name: "Altius-user", description: nil}
])

Tenant.create!([{tenant_name: "Yantra", address_line1: "Coimbatore", address_line2: "Coimbatore", city: "Coimbatore", state: "TamilNadu", country:"India", pincode: "638301",  companytype_id:1, isactive: true}])
Role.create!([{role_name:"CEO", tenant_id:1},{role_name:"Operator", tenant_id:1}])
User.create!(first_name: "Yantra", last_name: "24x7", email_id: "yantra@gmail.com", password: "yantra", phone_number: "9080767654", remarks: "test", usertype_id: 2, tenant_id:1, role_id:1, default: "yantra")   
