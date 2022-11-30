<?php

namespace Database\Seeders;

use Carbon\Carbon;
use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class UserSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $users = [
            [
                'name' => 'Luigi', 
                'email' => 'luigi.caradonna@gmail.com',
                'email_verified_at' => Carbon::now(),
                'password' => bcrypt('kadath')
            ]
        ];

        DB::table('users')->insert($users);
    }
}
