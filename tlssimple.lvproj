<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="16008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="NI.SortType" Type="Int">3</Property>
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="lv tls dll.lvlib" Type="Library" URL="../libretls/lv tls dll.lvlib"/>
		<Item Name="lvlibretls client socket.lvclass" Type="LVClass" URL="../libretls/client/lvlibretls client socket.lvclass"/>
		<Item Name="lvlibretls configurable context.lvclass" Type="LVClass" URL="../libretls/context parent/lvlibretls configurable context.lvclass"/>
		<Item Name="lvlibretls connection.lvclass" Type="LVClass" URL="../libretls/socket connection/lvlibretls connection.lvclass"/>
		<Item Name="lvlibretls server listener.lvclass" Type="LVClass" URL="../libretls/server/lvlibretls server listener.lvclass"/>
		<Item Name="lvlibcrypto digest.lvclass" Type="LVClass" URL="../libretls/crypto digest/lvlibcrypto digest.lvclass"/>
		<Item Name="server hello world.vi" Type="VI" URL="../test/server hello world.vi"/>
		<Item Name="digest test.vi" Type="VI" URL="../test/digest test.vi"/>
		<Item Name="simpletls bench.vi" Type="VI" URL="../test/simpletls bench.vi"/>
		<Item Name="simpletls test1.vi" Type="VI" URL="../test/simpletls test1.vi"/>
		<Item Name="simpletls test platsock.vi" Type="VI" URL="../test/simpletls test platsock.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="High Resolution Relative Seconds.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/High Resolution Relative Seconds.vi"/>
				<Item Name="Internecine Avoider.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/tcp.llb/Internecine Avoider.vi"/>
				<Item Name="TCP Listen Internal List.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/tcp.llb/TCP Listen Internal List.vi"/>
				<Item Name="TCP Listen List Operations.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/tcp.llb/TCP Listen List Operations.ctl"/>
				<Item Name="TCP Listen.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/tcp.llb/TCP Listen.vi"/>
			</Item>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
